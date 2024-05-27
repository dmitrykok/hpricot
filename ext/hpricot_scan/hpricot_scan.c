#line 1 "hpricot_scan.rl"
/*
* hpricot_scan.rl
*
* $Author: why $
* $Date: 2006-05-08 22:03:50 -0600 (Mon, 08 May 2006) $
*
* Copyright (C) 2006, 2010 why the lucky stiff
*/
#include <ruby.h>
#include <assert.h>

struct hpricot_struct {
	int len;
	VALUE* ptr;
};

#ifndef RARRAY_LEN
#define RARRAY_LEN(arr)  RARRAY(arr)->len
#define RSTRING_LEN(str) RSTRING(str)->len
#define RSTRING_PTR(str) RSTRING(str)->ptr
#endif

// VALUE hpricot_css(VALUE, VALUE, VALUE, VALUE, VALUE);
VALUE hpricot_css(VALUE, VALUE, VALUE, VALUE);

#define NO_WAY_SERIOUSLY "*** This should not happen, please file a bug report with the HTML you're parsing at http://github.com/hpricot/hpricot/issues.  So sorry!"

static VALUE sym_xmldecl, sym_doctype, sym_procins, sym_stag, sym_etag, sym_emptytag, sym_comment,
sym_cdata, sym_name, sym_parent, sym_raw_attributes, sym_raw_string, sym_tagno,
sym_allowed, sym_text, sym_children, sym_EMPTY, sym_CDATA;
static VALUE mHpricot, rb_eHpricotParseError;
static VALUE cBogusETag, cCData, cComment, cDoc, cDocType, cElem, cText,
cXMLDecl, cProcIns, symAllow, symDeny;
static ID s_ElementContent;
static ID s_downcase, s_new, s_parent, s_read, s_to_str;
static VALUE reProcInsParse;

#define H_ELE_TAG      0
#define H_ELE_PARENT   1
#define H_ELE_ATTR     2
#define H_ELE_ETAG     3
#define H_ELE_RAW      4
#define H_ELE_EC       5
#define H_ELE_HASH     6
#define H_ELE_CHILDREN 7

#define HSTRUCT_PTR(ele) ((struct hpricot_struct*)DATA_PTR(ele))->ptr

#define H_ELE_GET(ele, idx)      HSTRUCT_PTR(ele)[idx]
#define H_ELE_SET(ele, idx, val) HSTRUCT_PTR(ele)[idx] = val

#define OPT(opts, key) (!NIL_P(opts) && RTEST(rb_hash_aref(opts, ID2SYM(rb_intern("" # key)))))

#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
# define ASSOCIATE_INDEX(s)  rb_enc_associate_index((s), encoding_index)
# define ENCODING_INDEX     , encoding_index
#else
# define ASSOCIATE_INDEX(s)
# define ENCODING_INDEX
#endif

#define ELE(N)                                                          \
if (te > ts || text == 1) {                                           \
	char *raw = NULL;                                                   \
	int rawlen = 0;                                                     \
	ele_open = 0; text = 0;                                             \
	if (ts != 0 && sym_##N != sym_cdata && sym_##N != sym_text && sym_##N != sym_procins && sym_##N != sym_comment) { \
		raw = ts; rawlen = te - ts;                                       \
	}                                                                   \
	if (rb_block_given_p()) {                                           \
		VALUE raw_string = Qnil;                                          \
		if (raw != NULL) {                                                \
			raw_string = rb_str_new(raw, rawlen);                           \
			ASSOCIATE_INDEX(raw_string);                                    \
		}                                                                 \
		rb_yield_tokens(sym_##N, tag, attr, Qnil, taint);                 \
	} else                                                              \
	rb_hpricot_token(S, sym_##N, tag, attr, raw, rawlen, taint ENCODING_INDEX); \
}

#define SET(N, E)                               \
if (mark_##N == NULL || E == mark_##N) {      \
	N = rb_str_new2("");                        \
	ASSOCIATE_INDEX(N);                         \
} else if (E > mark_##N) {                    \
	N = rb_str_new(mark_##N, E - mark_##N);     \
	ASSOCIATE_INDEX(N);                         \
}

#define CAT(N, E) if (NIL_P(N)) { SET(N, E); } else { rb_str_cat(N, mark_##N, E - mark_##N); }

#define SLIDE(N) if (mark_##N > ts) mark_##N = buf + (mark_##N - ts);

#define ATTR(K, V) \
if (!NIL_P(K)) { \
	if (NIL_P(attr)) attr = rb_hash_new(); \
		rb_hash_aset(attr, K, V); \
}

#define TEXT_PASS() \
if (text == 0) \
	{ \
	if (ele_open == 1) { \
		ele_open = 0; \
		if (ts > 0) { \
			mark_tag = ts; \
		} \
	} else { \
		mark_tag = p; \
	} \
	attr = Qnil; \
	tag = Qnil; \
	text = 1; \
}

#define EBLK(N, T) CAT(tag, p - T + 1); ELE(N);


#line 176 "hpricot_scan.rl"



#line 122 "hpricot_scan.c"
static const int hpricot_scan_start = 198;
static const int hpricot_scan_error = -1;

static const int hpricot_scan_en_html_comment = 208;
static const int hpricot_scan_en_html_cdata = 210;
static const int hpricot_scan_en_html_procins = 212;
static const int hpricot_scan_en_main = 198;


#line 178 "hpricot_scan.rl"


#define BUFSIZE 16384

void rb_yield_tokens(VALUE sym, VALUE tag, VALUE attr, VALUE raw, int taint)
{
	VALUE ary;
	if (sym == sym_text) {
		raw = tag;
	}
	ary = rb_ary_new3(4, sym, tag, attr, raw);
	if (taint) {
		OBJ_TAINT(ary);
		OBJ_TAINT(tag);
		OBJ_TAINT(attr);
		OBJ_TAINT(raw);
	}
	rb_yield(ary);
}

#ifndef RHASH_TBL
/* rb_hash_lookup() is only in Ruby 1.8.7 */
static VALUE
our_rb_hash_lookup(VALUE hash, VALUE key)
{
	VALUE val;
	
	if (!st_lookup(RHASH(hash)->tbl, key, &val)) {
		return Qnil; /* without Hash#default */
	}
	
	return val;
}
#define rb_hash_lookup our_rb_hash_lookup
#endif

static void
rb_hpricot_add(VALUE focus, VALUE ele)
{
	VALUE children = H_ELE_GET(focus, H_ELE_CHILDREN);
	if (NIL_P(children))
		H_ELE_SET(focus, H_ELE_CHILDREN, (children = rb_ary_new2(1)));
	rb_ary_push(children, ele);
	H_ELE_SET(ele, H_ELE_PARENT, focus);
}

typedef struct {
	VALUE doc;
	VALUE focus;
	VALUE last;
	VALUE EC;
	unsigned char xml, strict, fixup;
} hpricot_state;

#define H_PROP(prop, idx) \
static VALUE hpricot_ele_set_##prop(VALUE self, VALUE x) { \
	H_ELE_SET(self, idx, x); \
	return self; \
} \
static VALUE hpricot_ele_clear_##prop(VALUE self) { \
	H_ELE_SET(self, idx, Qnil); \
	return Qtrue; \
} \
static VALUE hpricot_ele_get_##prop(VALUE self) { \
	return H_ELE_GET(self, idx); \
}

#define H_ATTR(prop) \
static VALUE hpricot_ele_set_##prop(VALUE self, VALUE x) { \
	rb_hash_aset(H_ELE_GET(self, H_ELE_ATTR), ID2SYM(rb_intern("" # prop)), x); \
	return self; \
} \
static VALUE hpricot_ele_get_##prop(VALUE self) { \
	return rb_hash_aref(H_ELE_GET(self, H_ELE_ATTR), ID2SYM(rb_intern("" # prop))); \
}

H_PROP(name, H_ELE_TAG);
H_PROP(raw, H_ELE_RAW);
H_PROP(parent, H_ELE_PARENT);
H_PROP(attr, H_ELE_ATTR);
H_PROP(etag, H_ELE_ETAG);
H_PROP(children, H_ELE_CHILDREN);
H_ATTR(target);
H_ATTR(encoding);
H_ATTR(version);
H_ATTR(standalone);
H_ATTR(system_id);
H_ATTR(public_id);

#define H_ELE(klass)                                                    \
ele = rb_obj_alloc(klass);                                            \
if (klass == cElem) {                                                 \
	H_ELE_SET(ele, H_ELE_TAG, tag);                                     \
	H_ELE_SET(ele, H_ELE_ATTR, attr);                                   \
	H_ELE_SET(ele, H_ELE_EC, ec);                                       \
	if (raw != NULL && (sym == sym_emptytag || sym == sym_stag || sym == sym_doctype)) { \
		VALUE raw_str = rb_str_new(raw, rawlen);                          \
		ASSOCIATE_INDEX(raw_str);                                         \
		H_ELE_SET(ele, H_ELE_RAW, raw_str);                               \
	}                                                                   \
} else if (klass == cDocType || klass == cProcIns || klass == cXMLDecl || klass == cBogusETag) { \
	if (klass == cBogusETag) {                                          \
		H_ELE_SET(ele, H_ELE_TAG, tag);                                   \
		if (raw != NULL) {                                                \
			VALUE raw_str = rb_str_new(raw, rawlen);                        \
			ASSOCIATE_INDEX(raw_str);                                       \
			H_ELE_SET(ele, H_ELE_ATTR, raw_str);                            \
		}                                                                 \
	} else {                                                            \
		if (klass == cDocType)                                            \
			ATTR(ID2SYM(rb_intern("target")), tag);                         \
		H_ELE_SET(ele, H_ELE_ATTR, attr);                                 \
		if (klass != cProcIns) {                                          \
			tag = Qnil;                                                     \
			if (raw != NULL) {                                              \
				tag = rb_str_new(raw, rawlen);                                \
				ASSOCIATE_INDEX(tag);                                         \
			}                                                               \
		}                                                                 \
		H_ELE_SET(ele, H_ELE_TAG, tag);                                   \
	}                                                                   \
} else {                                                              \
	H_ELE_SET(ele, H_ELE_TAG, tag);                                     \
}                                                                     \
S->last = ele

//
// the swift, compact parser logic.  most of the complicated stuff is done
// in the lexer.  this step just pairs up the start and end tags.
//
void
rb_hpricot_token(hpricot_state *S, VALUE sym, VALUE tag, VALUE attr,
char *raw, int rawlen, int taint
#ifdef HAVE_RUBY_ENCODING_H
, int encoding_index
#endif
)
{
	VALUE ele, ec = Qnil;
	
	//
	// in html mode, fix up start tags incorrectly formed as empty tags
	//
	if (!S->xml) {
		if (sym == sym_emptytag || sym == sym_stag || sym == sym_etag) {
			ec = rb_hash_aref(S->EC, tag);
			if (NIL_P(ec)) {
				tag = rb_funcall(tag, s_downcase, 0);
				ec = rb_hash_aref(S->EC, tag);
			}
		}
		
		if (H_ELE_GET(S->focus, H_ELE_EC) == sym_CDATA &&
			(sym != sym_procins && sym != sym_comment && sym != sym_cdata && sym != sym_text) &&
		!(sym == sym_etag && INT2FIX(rb_str_hash(tag)) == H_ELE_GET(S->focus, H_ELE_HASH)))
		{
			sym = sym_text;
			tag = rb_str_new(raw, rawlen);
			ASSOCIATE_INDEX(tag);
		}
		
		if (!NIL_P(ec)) {
			if (sym == sym_emptytag) {
				if (ec != sym_EMPTY)
					sym = sym_stag;
			} else if (sym == sym_stag) {
				if (ec == sym_EMPTY)
					sym = sym_emptytag;
			}
		}
	}
	
	if (sym == sym_emptytag || sym == sym_stag) {
		VALUE name = INT2FIX(rb_str_hash(tag));
		H_ELE(cElem);
		H_ELE_SET(ele, H_ELE_HASH, name);
		
		if (!S->xml) {
			VALUE match = Qnil, e = S->focus;
			while (e != S->doc)
			{
				if (ec == Qnil) {
					// anything can contain unknown elements
					if (match == Qnil)
						match = e;
				} else {
					VALUE hEC = H_ELE_GET(e, H_ELE_EC);
					
					if (TYPE(hEC) == T_HASH)
						{
						VALUE has = rb_hash_lookup(hEC, name);
						if (has != Qnil) {
							if (has == Qtrue) {
								if (match == Qnil)
									match = e;
							} else if (has == symAllow) {
								match = S->focus;
							} else if (has == symDeny) {
								match = Qnil;
							}
						}
					} else {
						// Unknown elements can contain anything
						if (match == Qnil)
							match = e;
					}
				}
				e = H_ELE_GET(e, H_ELE_PARENT);
			}
			
			if (match == Qnil)
				match = S->focus;
			S->focus = match;
		}
		
		rb_hpricot_add(S->focus, ele);
		
		//
		// in the case of a start tag that should be empty, just
		// skip the step that focuses the element.  focusing moves
		// us deeper into the document.
		//
		if (sym == sym_stag) {
			if (S->xml || ec != sym_EMPTY) {
				S->focus = ele;
				S->last = Qnil;
			}
		}
	} else if (sym == sym_etag) {
		VALUE name, match = Qnil, e = S->focus;
		if (S->strict) {
			if (NIL_P(rb_hash_aref(S->EC, tag))) {
				tag = rb_str_new2("div");
				ASSOCIATE_INDEX(tag);
			}
		}
		
		//
		// another optimization will be to improve this very simple
		// O(n) tag search, where n is the depth of the focused tag.
		//
		// (see also: the search above for fixups)
		//
		name = INT2FIX(rb_str_hash(tag));
		while (e != S->doc)
		{
			if (H_ELE_GET(e, H_ELE_HASH) == name)
				{
				match = e;
				break;
			}
			
			e = H_ELE_GET(e, H_ELE_PARENT);
		}
		
		if (NIL_P(match))
			{
			H_ELE(cBogusETag);
			rb_hpricot_add(S->focus, ele);
		}
		else
			{
			VALUE ele = Qnil;
			if (raw != NULL) {
				ele = rb_str_new(raw, rawlen);
				ASSOCIATE_INDEX(ele);
			}
			H_ELE_SET(match, H_ELE_ETAG, ele);
			S->focus = H_ELE_GET(match, H_ELE_PARENT);
			S->last = Qnil;
		}
	} else if (sym == sym_cdata) {
		H_ELE(cCData);
		rb_hpricot_add(S->focus, ele);
	} else if (sym == sym_comment) {
		H_ELE(cComment);
		rb_hpricot_add(S->focus, ele);
	} else if (sym == sym_doctype) {
		H_ELE(cDocType);
		if (S->strict) {
			VALUE id;
			id = rb_str_new2("http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd");
			ASSOCIATE_INDEX(id);
			rb_hash_aset(attr, ID2SYM(rb_intern("system_id")), id);
			id = rb_str_new2("-//W3C//DTD XHTML 1.0 Strict//EN");
			ASSOCIATE_INDEX(id);
			rb_hash_aset(attr, ID2SYM(rb_intern("public_id")), id);
		}
		rb_hpricot_add(S->focus, ele);
	} else if (sym == sym_procins) {
		VALUE match = rb_funcall(tag, rb_intern("match"), 1, reProcInsParse);
		tag = rb_reg_nth_match(1, match);
		attr = rb_reg_nth_match(2, match);
		{
			H_ELE(cProcIns);
			rb_hpricot_add(S->focus, ele);
		}
	} else if (sym == sym_text) {
		// TODO: add raw_string as well?
		if (!NIL_P(S->last) && RTEST(rb_obj_is_instance_of(S->last, cText))) {
			rb_str_append(H_ELE_GET(S->last, H_ELE_TAG), tag);
		} else {
			H_ELE(cText);
			rb_hpricot_add(S->focus, ele);
		}
	} else if (sym == sym_xmldecl) {
		H_ELE(cXMLDecl);
		rb_hpricot_add(S->focus, ele);
	}
}

VALUE hpricot_scan(int argc, VALUE *argv, VALUE self)
{
	int cs, act, have = 0, nread = 0, curline = 1, text = 0, io = 0;
	char *ts = 0, *te = 0, *buf = NULL, *eof = NULL;
	
	hpricot_state *S = NULL;
	VALUE port, opts;
	VALUE attr = Qnil, tag = Qnil, akey = Qnil, aval = Qnil, bufsize = Qnil;
	char *mark_tag = 0, *mark_akey = 0, *mark_aval = 0;
	int done = 0, ele_open = 0, buffer_size = 0, taint = 0;
#ifdef HAVE_RUBY_ENCODING_H
	int encoding_index = rb_enc_to_index(rb_default_external_encoding());
#endif
	
	rb_scan_args(argc, argv, "11", &port, &opts);
	taint = OBJ_TAINTED(port);
	io = rb_respond_to(port, s_read);
	if (!io)
		{
		if (rb_respond_to(port, s_to_str))
			{
			port = rb_funcall(port, s_to_str, 0);
			StringValue(port);
		}
		else
			{
			rb_raise(rb_eArgError, "an Hpricot document must be built from an input source (a String or IO object.)");
		}
	}
	
	if (TYPE(opts) != T_HASH)
		opts = Qnil;
	
	if (!rb_block_given_p())
		{
		S = ALLOC(hpricot_state);
		S->doc = rb_obj_alloc(cDoc);
		rb_gc_register_address(&S->doc);
		S->focus = S->doc;
		S->last = Qnil;
		S->xml = OPT(opts, xml);
		S->strict = OPT(opts, xhtml_strict);
		S->fixup = OPT(opts, fixup_tags);
		if (S->strict) S->fixup = 1;
			rb_ivar_set(S->doc, rb_intern("@options"), opts);
		
		S->EC = rb_const_get(mHpricot, s_ElementContent);
	}
	
	buffer_size = BUFSIZE;
	if (rb_ivar_defined(self, rb_intern("@buffer_size")) == Qtrue) {
		bufsize = rb_ivar_get(self, rb_intern("@buffer_size"));
		if (!NIL_P(bufsize)) {
			buffer_size = NUM2INT(bufsize);
		}
	}
	
	if (io)
		buf = ALLOC_N(char, buffer_size);
	

#line 502 "hpricot_scan.c"
	{
		cs = (int)hpricot_scan_start;
		ts = 0;
		te = 0;
		act = 0;
	}
	
#line 549 "hpricot_scan.rl"

	
	while (!done) {
		VALUE str;
		char *p, *pe;
		int len, space = buffer_size - have, tokstart_diff, tokend_diff, mark_tag_diff, mark_akey_diff, mark_aval_diff;
		
		if (io)
			{
			if (space == 0) {
				/* We've used up the entire buffer storing an already-parsed token
				* prefix that must be preserved.  Likely caused by super-long attributes.
				* Increase buffer size and continue  */
				tokstart_diff = ts - buf;
				tokend_diff = te - buf;
				mark_tag_diff = mark_tag - buf;
				mark_akey_diff = mark_akey - buf;
				mark_aval_diff = mark_aval - buf;
				
				buffer_size += BUFSIZE;
				REALLOC_N(buf, char, buffer_size);
				
				space = buffer_size - have;
				
				ts = buf + tokstart_diff;
				te = buf + tokend_diff;
				mark_tag = buf + mark_tag_diff;
				mark_akey = buf + mark_akey_diff;
				mark_aval = buf + mark_aval_diff;
			}
			p = buf + have;
			
			str = rb_funcall(port, s_read, 1, INT2FIX(space));
			len = RSTRING_LEN(str);
			memcpy(p, StringValuePtr(str), len);
		}
		else
			{
			p = RSTRING_PTR(port);
			len = RSTRING_LEN(port) + 1;
			done = 1;
		}
		
		nread += len;
		
		/* If this is the last buffer, tack on an EOF. */
		if (io && len < space) {
			p[len++] = 0;
			done = 1;
		}
		
		pe = p + len;

#line 561 "hpricot_scan.c"
{
			switch ( cs ) {
				case 198:
				goto st_case_198;
				case 199:
				goto st_case_199;
				case 0:
				goto st_case_0;
				case 1:
				goto st_case_1;
				case 2:
				goto st_case_2;
				case 3:
				goto st_case_3;
				case 4:
				goto st_case_4;
				case 5:
				goto st_case_5;
				case 6:
				goto st_case_6;
				case 7:
				goto st_case_7;
				case 8:
				goto st_case_8;
				case 9:
				goto st_case_9;
				case 10:
				goto st_case_10;
				case 11:
				goto st_case_11;
				case 12:
				goto st_case_12;
				case 13:
				goto st_case_13;
				case 14:
				goto st_case_14;
				case 15:
				goto st_case_15;
				case 16:
				goto st_case_16;
				case 17:
				goto st_case_17;
				case 18:
				goto st_case_18;
				case 19:
				goto st_case_19;
				case 20:
				goto st_case_20;
				case 21:
				goto st_case_21;
				case 22:
				goto st_case_22;
				case 23:
				goto st_case_23;
				case 24:
				goto st_case_24;
				case 25:
				goto st_case_25;
				case 26:
				goto st_case_26;
				case 27:
				goto st_case_27;
				case 28:
				goto st_case_28;
				case 29:
				goto st_case_29;
				case 30:
				goto st_case_30;
				case 31:
				goto st_case_31;
				case 32:
				goto st_case_32;
				case 33:
				goto st_case_33;
				case 34:
				goto st_case_34;
				case 35:
				goto st_case_35;
				case 36:
				goto st_case_36;
				case 37:
				goto st_case_37;
				case 38:
				goto st_case_38;
				case 39:
				goto st_case_39;
				case 200:
				goto st_case_200;
				case 40:
				goto st_case_40;
				case 41:
				goto st_case_41;
				case 201:
				goto st_case_201;
				case 42:
				goto st_case_42;
				case 43:
				goto st_case_43;
				case 202:
				goto st_case_202;
				case 44:
				goto st_case_44;
				case 45:
				goto st_case_45;
				case 46:
				goto st_case_46;
				case 47:
				goto st_case_47;
				case 48:
				goto st_case_48;
				case 49:
				goto st_case_49;
				case 50:
				goto st_case_50;
				case 51:
				goto st_case_51;
				case 52:
				goto st_case_52;
				case 53:
				goto st_case_53;
				case 54:
				goto st_case_54;
				case 55:
				goto st_case_55;
				case 56:
				goto st_case_56;
				case 57:
				goto st_case_57;
				case 58:
				goto st_case_58;
				case 59:
				goto st_case_59;
				case 60:
				goto st_case_60;
				case 61:
				goto st_case_61;
				case 62:
				goto st_case_62;
				case 63:
				goto st_case_63;
				case 64:
				goto st_case_64;
				case 65:
				goto st_case_65;
				case 66:
				goto st_case_66;
				case 67:
				goto st_case_67;
				case 68:
				goto st_case_68;
				case 69:
				goto st_case_69;
				case 70:
				goto st_case_70;
				case 71:
				goto st_case_71;
				case 72:
				goto st_case_72;
				case 73:
				goto st_case_73;
				case 74:
				goto st_case_74;
				case 75:
				goto st_case_75;
				case 76:
				goto st_case_76;
				case 77:
				goto st_case_77;
				case 78:
				goto st_case_78;
				case 79:
				goto st_case_79;
				case 80:
				goto st_case_80;
				case 81:
				goto st_case_81;
				case 82:
				goto st_case_82;
				case 83:
				goto st_case_83;
				case 203:
				goto st_case_203;
				case 84:
				goto st_case_84;
				case 85:
				goto st_case_85;
				case 86:
				goto st_case_86;
				case 87:
				goto st_case_87;
				case 88:
				goto st_case_88;
				case 89:
				goto st_case_89;
				case 90:
				goto st_case_90;
				case 91:
				goto st_case_91;
				case 92:
				goto st_case_92;
				case 93:
				goto st_case_93;
				case 94:
				goto st_case_94;
				case 95:
				goto st_case_95;
				case 96:
				goto st_case_96;
				case 97:
				goto st_case_97;
				case 98:
				goto st_case_98;
				case 99:
				goto st_case_99;
				case 100:
				goto st_case_100;
				case 101:
				goto st_case_101;
				case 102:
				goto st_case_102;
				case 204:
				goto st_case_204;
				case 103:
				goto st_case_103;
				case 104:
				goto st_case_104;
				case 105:
				goto st_case_105;
				case 106:
				goto st_case_106;
				case 107:
				goto st_case_107;
				case 108:
				goto st_case_108;
				case 109:
				goto st_case_109;
				case 110:
				goto st_case_110;
				case 111:
				goto st_case_111;
				case 112:
				goto st_case_112;
				case 113:
				goto st_case_113;
				case 114:
				goto st_case_114;
				case 115:
				goto st_case_115;
				case 116:
				goto st_case_116;
				case 117:
				goto st_case_117;
				case 205:
				goto st_case_205;
				case 118:
				goto st_case_118;
				case 119:
				goto st_case_119;
				case 120:
				goto st_case_120;
				case 121:
				goto st_case_121;
				case 122:
				goto st_case_122;
				case 123:
				goto st_case_123;
				case 124:
				goto st_case_124;
				case 125:
				goto st_case_125;
				case 126:
				goto st_case_126;
				case 127:
				goto st_case_127;
				case 128:
				goto st_case_128;
				case 129:
				goto st_case_129;
				case 130:
				goto st_case_130;
				case 131:
				goto st_case_131;
				case 132:
				goto st_case_132;
				case 133:
				goto st_case_133;
				case 134:
				goto st_case_134;
				case 135:
				goto st_case_135;
				case 136:
				goto st_case_136;
				case 137:
				goto st_case_137;
				case 138:
				goto st_case_138;
				case 139:
				goto st_case_139;
				case 140:
				goto st_case_140;
				case 206:
				goto st_case_206;
				case 141:
				goto st_case_141;
				case 142:
				goto st_case_142;
				case 143:
				goto st_case_143;
				case 207:
				goto st_case_207;
				case 144:
				goto st_case_144;
				case 145:
				goto st_case_145;
				case 146:
				goto st_case_146;
				case 147:
				goto st_case_147;
				case 148:
				goto st_case_148;
				case 149:
				goto st_case_149;
				case 150:
				goto st_case_150;
				case 151:
				goto st_case_151;
				case 152:
				goto st_case_152;
				case 153:
				goto st_case_153;
				case 154:
				goto st_case_154;
				case 155:
				goto st_case_155;
				case 156:
				goto st_case_156;
				case 157:
				goto st_case_157;
				case 158:
				goto st_case_158;
				case 159:
				goto st_case_159;
				case 160:
				goto st_case_160;
				case 161:
				goto st_case_161;
				case 162:
				goto st_case_162;
				case 163:
				goto st_case_163;
				case 164:
				goto st_case_164;
				case 165:
				goto st_case_165;
				case 166:
				goto st_case_166;
				case 167:
				goto st_case_167;
				case 168:
				goto st_case_168;
				case 169:
				goto st_case_169;
				case 170:
				goto st_case_170;
				case 171:
				goto st_case_171;
				case 172:
				goto st_case_172;
				case 173:
				goto st_case_173;
				case 174:
				goto st_case_174;
				case 175:
				goto st_case_175;
				case 176:
				goto st_case_176;
				case 177:
				goto st_case_177;
				case 178:
				goto st_case_178;
				case 179:
				goto st_case_179;
				case 180:
				goto st_case_180;
				case 181:
				goto st_case_181;
				case 182:
				goto st_case_182;
				case 183:
				goto st_case_183;
				case 184:
				goto st_case_184;
				case 185:
				goto st_case_185;
				case 186:
				goto st_case_186;
				case 187:
				goto st_case_187;
				case 188:
				goto st_case_188;
				case 189:
				goto st_case_189;
				case 190:
				goto st_case_190;
				case 191:
				goto st_case_191;
				case 192:
				goto st_case_192;
				case 193:
				goto st_case_193;
				case 194:
				goto st_case_194;
				case 195:
				goto st_case_195;
				case 208:
				goto st_case_208;
				case 209:
				goto st_case_209;
				case 196:
				goto st_case_196;
				case 210:
				goto st_case_210;
				case 211:
				goto st_case_211;
				case 197:
				goto st_case_197;
				case 212:
				goto st_case_212;
				case 213:
				goto st_case_213;
			}
			_ctr0:
				{
#line 73 "hpricot_common.rl"
				{p = ((te))-1;
					{
#line 73 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 1001 "hpricot_scan.c"

			goto _st198;
			_ctr4:
				{
#line 71 "hpricot_common.rl"
				{te = p+1;{
#line 71 "hpricot_common.rl"
						{goto _st208;}}
				}}
			
#line 1011 "hpricot_scan.c"

			goto _st198;
			_ctr15:
				{
#line 136 "hpricot_scan.rl"
				SET(tag, p); }
			
#line 1018 "hpricot_scan.c"

				{
#line 66 "hpricot_common.rl"
				{te = p+1;{
#line 66 "hpricot_common.rl"
						ELE(doctype); }
				}}
			
#line 1026 "hpricot_scan.c"

			goto _st198;
			_ctr18:
				{
#line 66 "hpricot_common.rl"
				{te = p+1;{
#line 66 "hpricot_common.rl"
						ELE(doctype); }
				}}
			
#line 1036 "hpricot_scan.c"

			goto _st198;
			_ctr39:
				{
#line 1 "NONE"
				{switch( act ) {
						case 8:  {
							p = ((te))-1;
							{
#line 66 "hpricot_common.rl"
								ELE(doctype); }
							break; 
						}
						case 10:  {
							p = ((te))-1;
							{
#line 68 "hpricot_common.rl"
								ELE(stag); }
							break; 
						}
						case 12:  {
							p = ((te))-1;
							{
#line 70 "hpricot_common.rl"
								ELE(emptytag); }
							break; 
						}
						case 15:  {
							p = ((te))-1;
							{
#line 73 "hpricot_common.rl"
								TEXT_PASS(); }
							break; 
						}
					}}
			}
			
#line 1073 "hpricot_scan.c"

			goto _st198;
			_ctr93:
				{
#line 72 "hpricot_common.rl"
				{te = p+1;{
#line 72 "hpricot_common.rl"
						{goto _st210;}}
				}}
			
#line 1083 "hpricot_scan.c"

			goto _st198;
			_ctr97:
				{
#line 136 "hpricot_scan.rl"
				SET(tag, p); }
			
#line 1090 "hpricot_scan.c"

				{
#line 69 "hpricot_common.rl"
				{te = p+1;{
#line 69 "hpricot_common.rl"
						ELE(etag); }
				}}
			
#line 1098 "hpricot_scan.c"

			goto _st198;
			_ctr99:
				{
#line 69 "hpricot_common.rl"
				{te = p+1;{
#line 69 "hpricot_common.rl"
						ELE(etag); }
				}}
			
#line 1108 "hpricot_scan.c"

			goto _st198;
			_ctr103:
				{
#line 136 "hpricot_scan.rl"
				SET(tag, p); }
			
#line 1115 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{te = p+1;{
#line 68 "hpricot_common.rl"
						ELE(stag); }
				}}
			
#line 1123 "hpricot_scan.c"

			goto _st198;
			_ctr107:
				{
#line 68 "hpricot_common.rl"
				{te = p+1;{
#line 68 "hpricot_common.rl"
						ELE(stag); }
				}}
			
#line 1133 "hpricot_scan.c"

			goto _st198;
			_ctr112:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 1140 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 1149 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{te = p+1;{
#line 68 "hpricot_common.rl"
						ELE(stag); }
				}}
			
#line 1157 "hpricot_scan.c"

			goto _st198;
			_ctr117:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 1168 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{te = p+1;{
#line 68 "hpricot_common.rl"
						ELE(stag); }
				}}
			
#line 1176 "hpricot_scan.c"

			goto _st198;
			_ctr118:
				{
#line 70 "hpricot_common.rl"
				{te = p+1;{
#line 70 "hpricot_common.rl"
						ELE(emptytag); }
				}}
			
#line 1186 "hpricot_scan.c"

			goto _st198;
			_ctr129:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 1196 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 1205 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{te = p+1;{
#line 68 "hpricot_common.rl"
						ELE(stag); }
				}}
			
#line 1213 "hpricot_scan.c"

			goto _st198;
			_ctr133:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 1224 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 1232 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{te = p+1;{
#line 68 "hpricot_common.rl"
						ELE(stag); }
				}}
			
#line 1240 "hpricot_scan.c"

			goto _st198;
			_ctr139:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 1250 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 1255 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 1264 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{te = p+1;{
#line 68 "hpricot_common.rl"
						ELE(stag); }
				}}
			
#line 1272 "hpricot_scan.c"

			goto _st198;
			_ctr335:
				{
#line 67 "hpricot_common.rl"
				{p = ((te))-1;
					{
#line 67 "hpricot_common.rl"
						{goto _st212;}}
				}}
			
#line 1283 "hpricot_scan.c"

			goto _st198;
			_ctr349:
				{
#line 65 "hpricot_common.rl"
				{te = p+1;{
#line 65 "hpricot_common.rl"
						ELE(xmldecl); }
				}}
			
#line 1293 "hpricot_scan.c"

			goto _st198;
			_ctr398:
				{
#line 73 "hpricot_common.rl"
				{te = p+1;{
#line 73 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 1303 "hpricot_scan.c"

			goto _st198;
			_ctr399:
				{
#line 9 "hpricot_common.rl"
				curline += 1;}
			
#line 1310 "hpricot_scan.c"

				{
#line 73 "hpricot_common.rl"
				{te = p+1;{
#line 73 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 1318 "hpricot_scan.c"

			goto _st198;
			_ctr401:
				{
#line 73 "hpricot_common.rl"
				{te = p;p = p - 1;{
#line 73 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 1328 "hpricot_scan.c"

			goto _st198;
			_ctr406:
				{
#line 66 "hpricot_common.rl"
				{te = p;p = p - 1;{
#line 66 "hpricot_common.rl"
						ELE(doctype); }
				}}
			
#line 1338 "hpricot_scan.c"

			goto _st198;
			_ctr407:
				{
#line 67 "hpricot_common.rl"
				{te = p;p = p - 1;{
#line 67 "hpricot_common.rl"
						{goto _st212;}}
				}}
			
#line 1348 "hpricot_scan.c"

			goto _st198;
			_st198:
			if ( p == eof )
				goto _out198;
				{
#line 1 "NONE"
				{ts = 0;}}
			
#line 1357 "hpricot_scan.c"

			p+= 1;
			st_case_198:
			if ( p == pe && p != eof )
				goto _out198;
				{
#line 1 "NONE"
				{ts = p;}}
			
#line 1366 "hpricot_scan.c"

			if ( p == eof ) {
				goto _st198;}
			else {
				switch( ( (*( p))) ) {
					case 10: {
						goto _ctr399;
					}
					case 60: {
						goto _ctr400;
					}
				}
				goto _ctr398;
			}
			_ctr400:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 1385 "hpricot_scan.c"

				{
#line 121 "hpricot_scan.rl"
				
				if (text == 1) {
					CAT(tag, p);
					ELE(text);
					text = 0;
				}
				attr = Qnil;
				tag = Qnil;
				mark_tag = NULL;
				ele_open = 1;
			}
			
#line 1400 "hpricot_scan.c"

				{
#line 73 "hpricot_common.rl"
				{act = 15;}}
			
#line 1405 "hpricot_scan.c"

			goto _st199;
			_st199:
			if ( p == eof )
				goto _out199;
			p+= 1;
			st_case_199:
			if ( p == pe && p != eof )
				goto _out199;
			if ( p == eof ) {
				goto _ctr401;}
			else {
				switch( ( (*( p))) ) {
					case 33: {
						goto _st0;
					}
					case 47: {
						goto _st59;
					}
					case 58: {
						goto _ctr404;
					}
					case 63: {
						goto _st139;
					}
					case 95: {
						goto _ctr404;
					}
				}
				if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr404;
					}
				} else if ( ( (*( p))) >= 65 ) {
					goto _ctr404;
				}
				goto _ctr401;
			}
			_st0:
			if ( p == eof )
				goto _out0;
			p+= 1;
			st_case_0:
			if ( p == pe && p != eof )
				goto _out0;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 45: {
						goto _st1;
					}
					case 68: {
						goto _st2;
					}
					case 91: {
						goto _st53;
					}
				}
				goto _ctr0;
			}
			_st1:
			if ( p == eof )
				goto _out1;
			p+= 1;
			st_case_1:
			if ( p == pe && p != eof )
				goto _out1;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 45 ) {
					goto _ctr4;
				}
				goto _ctr0;
			}
			_st2:
			if ( p == eof )
				goto _out2;
			p+= 1;
			st_case_2:
			if ( p == pe && p != eof )
				goto _out2;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 79 ) {
					goto _st3;
				}
				goto _ctr0;
			}
			_st3:
			if ( p == eof )
				goto _out3;
			p+= 1;
			st_case_3:
			if ( p == pe && p != eof )
				goto _out3;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 67 ) {
					goto _st4;
				}
				goto _ctr0;
			}
			_st4:
			if ( p == eof )
				goto _out4;
			p+= 1;
			st_case_4:
			if ( p == pe && p != eof )
				goto _out4;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 84 ) {
					goto _st5;
				}
				goto _ctr0;
			}
			_st5:
			if ( p == eof )
				goto _out5;
			p+= 1;
			st_case_5:
			if ( p == pe && p != eof )
				goto _out5;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 89 ) {
					goto _st6;
				}
				goto _ctr0;
			}
			_st6:
			if ( p == eof )
				goto _out6;
			p+= 1;
			st_case_6:
			if ( p == pe && p != eof )
				goto _out6;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 80 ) {
					goto _st7;
				}
				goto _ctr0;
			}
			_st7:
			if ( p == eof )
				goto _out7;
			p+= 1;
			st_case_7:
			if ( p == pe && p != eof )
				goto _out7;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 69 ) {
					goto _st8;
				}
				goto _ctr0;
			}
			_st8:
			if ( p == eof )
				goto _out8;
			p+= 1;
			st_case_8:
			if ( p == pe && p != eof )
				goto _out8;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 32 ) {
					goto _st9;
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st9;
				}
				goto _ctr0;
			}
			_st9:
			if ( p == eof )
				goto _out9;
			p+= 1;
			st_case_9:
			if ( p == pe && p != eof )
				goto _out9;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st9;
					}
					case 58: {
						goto _ctr12;
					}
					case 95: {
						goto _ctr12;
					}
				}
				if ( ( (*( p))) < 65 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st9;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr12;
					}
				} else {
					goto _ctr12;
				}
				goto _ctr0;
			}
			_ctr12:
				{
#line 133 "hpricot_scan.rl"
				mark_tag = p; }
			
#line 1628 "hpricot_scan.c"

			goto _st10;
			_st10:
			if ( p == eof )
				goto _out10;
			p+= 1;
			st_case_10:
			if ( p == pe && p != eof )
				goto _out10;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr13;
					}
					case 62: {
						goto _ctr15;
					}
					case 63: {
						goto _st10;
					}
					case 91: {
						goto _ctr16;
					}
					case 95: {
						goto _st10;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( ( (*( p))) > 13 ) {
						if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
							goto _st10;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr13;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st10;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st10;
					}
				} else {
					goto _st10;
				}
				goto _ctr0;
			}
			_ctr13:
				{
#line 136 "hpricot_scan.rl"
				SET(tag, p); }
			
#line 1683 "hpricot_scan.c"

			goto _st11;
			_st11:
			if ( p == eof )
				goto _out11;
			p+= 1;
			st_case_11:
			if ( p == pe && p != eof )
				goto _out11;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st11;
					}
					case 62: {
						goto _ctr18;
					}
					case 80: {
						goto _st12;
					}
					case 83: {
						goto _st48;
					}
					case 91: {
						goto _st26;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st11;
				}
				goto _ctr0;
			}
			_st12:
			if ( p == eof )
				goto _out12;
			p+= 1;
			st_case_12:
			if ( p == pe && p != eof )
				goto _out12;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 85 ) {
					goto _st13;
				}
				goto _ctr0;
			}
			_st13:
			if ( p == eof )
				goto _out13;
			p+= 1;
			st_case_13:
			if ( p == pe && p != eof )
				goto _out13;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 66 ) {
					goto _st14;
				}
				goto _ctr0;
			}
			_st14:
			if ( p == eof )
				goto _out14;
			p+= 1;
			st_case_14:
			if ( p == pe && p != eof )
				goto _out14;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 76 ) {
					goto _st15;
				}
				goto _ctr0;
			}
			_st15:
			if ( p == eof )
				goto _out15;
			p+= 1;
			st_case_15:
			if ( p == pe && p != eof )
				goto _out15;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 73 ) {
					goto _st16;
				}
				goto _ctr0;
			}
			_st16:
			if ( p == eof )
				goto _out16;
			p+= 1;
			st_case_16:
			if ( p == pe && p != eof )
				goto _out16;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 67 ) {
					goto _st17;
				}
				goto _ctr0;
			}
			_st17:
			if ( p == eof )
				goto _out17;
			p+= 1;
			st_case_17:
			if ( p == pe && p != eof )
				goto _out17;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 32 ) {
					goto _st18;
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st18;
				}
				goto _ctr0;
			}
			_st18:
			if ( p == eof )
				goto _out18;
			p+= 1;
			st_case_18:
			if ( p == pe && p != eof )
				goto _out18;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st18;
					}
					case 34: {
						goto _st19;
					}
					case 39: {
						goto _st30;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st18;
				}
				goto _ctr0;
			}
			_st19:
			if ( p == eof )
				goto _out19;
			p+= 1;
			st_case_19:
			if ( p == pe && p != eof )
				goto _out19;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr30;
					}
					case 34: {
						goto _ctr31;
					}
					case 61: {
						goto _ctr30;
					}
					case 95: {
						goto _ctr30;
					}
				}
				if ( ( (*( p))) < 39 ) {
					if ( 32 <= ( (*( p))) && ( (*( p))) <= 37 ) {
						goto _ctr30;
					}
				} else if ( ( (*( p))) > 59 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr30;
						}
					} else if ( ( (*( p))) >= 63 ) {
						goto _ctr30;
					}
				} else {
					goto _ctr30;
				}
				goto _ctr0;
			}
			_ctr30:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 1882 "hpricot_scan.c"

			goto _st20;
			_st20:
			if ( p == eof )
				goto _out20;
			p+= 1;
			st_case_20:
			if ( p == pe && p != eof )
				goto _out20;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 9: {
						goto _st20;
					}
					case 34: {
						goto _ctr33;
					}
					case 61: {
						goto _st20;
					}
					case 95: {
						goto _st20;
					}
				}
				if ( ( (*( p))) < 39 ) {
					if ( 32 <= ( (*( p))) && ( (*( p))) <= 37 ) {
						goto _st20;
					}
				} else if ( ( (*( p))) > 59 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st20;
						}
					} else if ( ( (*( p))) >= 63 ) {
						goto _st20;
					}
				} else {
					goto _st20;
				}
				goto _ctr0;
			}
			_ctr31:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 1930 "hpricot_scan.c"

				{
#line 158 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("public_id")), aval); }
			
#line 1935 "hpricot_scan.c"

			goto _st21;
			_ctr33:
				{
#line 158 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("public_id")), aval); }
			
#line 1942 "hpricot_scan.c"

			goto _st21;
			_st21:
			if ( p == eof )
				goto _out21;
			p+= 1;
			st_case_21:
			if ( p == pe && p != eof )
				goto _out21;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st22;
					}
					case 62: {
						goto _ctr18;
					}
					case 91: {
						goto _st26;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st22;
				}
				goto _ctr0;
			}
			_st22:
			if ( p == eof )
				goto _out22;
			p+= 1;
			st_case_22:
			if ( p == pe && p != eof )
				goto _out22;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st22;
					}
					case 34: {
						goto _st23;
					}
					case 39: {
						goto _st28;
					}
					case 62: {
						goto _ctr18;
					}
					case 91: {
						goto _st26;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st22;
				}
				goto _ctr0;
			}
			_st23:
			if ( p == eof )
				goto _out23;
			p+= 1;
			st_case_23:
			if ( p == pe && p != eof )
				goto _out23;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 34 ) {
					goto _ctr38;
				}
				goto _ctr37;
			}
			_ctr37:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2022 "hpricot_scan.c"

			goto _st24;
			_st24:
			if ( p == eof )
				goto _out24;
			p+= 1;
			st_case_24:
			if ( p == pe && p != eof )
				goto _out24;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				if ( ( (*( p))) == 34 ) {
					goto _ctr41;
				}
				goto _st24;
			}
			_ctr38:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2044 "hpricot_scan.c"

				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2049 "hpricot_scan.c"

			goto _st25;
			_ctr41:
				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2056 "hpricot_scan.c"

			goto _st25;
			_st25:
			if ( p == eof )
				goto _out25;
			p+= 1;
			st_case_25:
			if ( p == pe && p != eof )
				goto _out25;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st25;
					}
					case 62: {
						goto _ctr18;
					}
					case 91: {
						goto _st26;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st25;
				}
				goto _ctr39;
			}
			_ctr16:
				{
#line 136 "hpricot_scan.rl"
				SET(tag, p); }
			
#line 2089 "hpricot_scan.c"

			goto _st26;
			_st26:
			if ( p == eof )
				goto _out26;
			p+= 1;
			st_case_26:
			if ( p == pe && p != eof )
				goto _out26;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				if ( ( (*( p))) == 93 ) {
					goto _st27;
				}
				goto _st26;
			}
			_st27:
			if ( p == eof )
				goto _out27;
			p+= 1;
			st_case_27:
			if ( p == pe && p != eof )
				goto _out27;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st27;
					}
					case 62: {
						goto _ctr18;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st27;
				}
				goto _ctr39;
			}
			_st28:
			if ( p == eof )
				goto _out28;
			p+= 1;
			st_case_28:
			if ( p == pe && p != eof )
				goto _out28;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 39 ) {
					goto _ctr38;
				}
				goto _ctr44;
			}
			_ctr44:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2149 "hpricot_scan.c"

			goto _st29;
			_st29:
			if ( p == eof )
				goto _out29;
			p+= 1;
			st_case_29:
			if ( p == pe && p != eof )
				goto _out29;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				if ( ( (*( p))) == 39 ) {
					goto _ctr41;
				}
				goto _st29;
			}
			_st30:
			if ( p == eof )
				goto _out30;
			p+= 1;
			st_case_30:
			if ( p == pe && p != eof )
				goto _out30;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr46;
					}
					case 39: {
						goto _ctr47;
					}
					case 61: {
						goto _ctr46;
					}
					case 95: {
						goto _ctr46;
					}
				}
				if ( ( (*( p))) < 40 ) {
					if ( ( (*( p))) > 33 ) {
						if ( 35 <= ( (*( p))) && ( (*( p))) <= 37 ) {
							goto _ctr46;
						}
					} else if ( ( (*( p))) >= 32 ) {
						goto _ctr46;
					}
				} else if ( ( (*( p))) > 59 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr46;
						}
					} else if ( ( (*( p))) >= 63 ) {
						goto _ctr46;
					}
				} else {
					goto _ctr46;
				}
				goto _ctr0;
			}
			_ctr46:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2216 "hpricot_scan.c"

			goto _st31;
			_st31:
			if ( p == eof )
				goto _out31;
			p+= 1;
			st_case_31:
			if ( p == pe && p != eof )
				goto _out31;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 9: {
						goto _st31;
					}
					case 39: {
						goto _ctr49;
					}
					case 61: {
						goto _st31;
					}
					case 95: {
						goto _st31;
					}
				}
				if ( ( (*( p))) < 40 ) {
					if ( ( (*( p))) > 33 ) {
						if ( 35 <= ( (*( p))) && ( (*( p))) <= 37 ) {
							goto _st31;
						}
					} else if ( ( (*( p))) >= 32 ) {
						goto _st31;
					}
				} else if ( ( (*( p))) > 59 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st31;
						}
					} else if ( ( (*( p))) >= 63 ) {
						goto _st31;
					}
				} else {
					goto _st31;
				}
				goto _ctr0;
			}
			_ctr47:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2268 "hpricot_scan.c"

				{
#line 158 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("public_id")), aval); }
			
#line 2273 "hpricot_scan.c"

			goto _st32;
			_ctr49:
				{
#line 158 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("public_id")), aval); }
			
#line 2280 "hpricot_scan.c"

			goto _st32;
			_ctr55:
				{
#line 158 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("public_id")), aval); }
			
#line 2287 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2292 "hpricot_scan.c"

				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2297 "hpricot_scan.c"

			goto _st32;
			_ctr82:
				{
#line 158 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("public_id")), aval); }
			
#line 2304 "hpricot_scan.c"

				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2309 "hpricot_scan.c"

			goto _st32;
			_st32:
			if ( p == eof )
				goto _out32;
			p+= 1;
			st_case_32:
			if ( p == pe && p != eof )
				goto _out32;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 9: {
						goto _st33;
					}
					case 32: {
						goto _st33;
					}
					case 33: {
						goto _st31;
					}
					case 39: {
						goto _ctr49;
					}
					case 62: {
						goto _ctr18;
					}
					case 91: {
						goto _st26;
					}
					case 95: {
						goto _st31;
					}
				}
				if ( ( (*( p))) < 40 ) {
					if ( ( (*( p))) > 13 ) {
						if ( 35 <= ( (*( p))) && ( (*( p))) <= 37 ) {
							goto _st31;
						}
					} else if ( ( (*( p))) >= 10 ) {
						goto _st22;
					}
				} else if ( ( (*( p))) > 59 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st31;
						}
					} else if ( ( (*( p))) >= 61 ) {
						goto _st31;
					}
				} else {
					goto _st31;
				}
				goto _ctr0;
			}
			_st33:
			if ( p == eof )
				goto _out33;
			p+= 1;
			st_case_33:
			if ( p == pe && p != eof )
				goto _out33;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 9: {
						goto _st33;
					}
					case 32: {
						goto _st33;
					}
					case 34: {
						goto _st23;
					}
					case 39: {
						goto _ctr51;
					}
					case 62: {
						goto _ctr18;
					}
					case 91: {
						goto _st26;
					}
					case 95: {
						goto _st31;
					}
				}
				if ( ( (*( p))) < 40 ) {
					if ( ( (*( p))) > 13 ) {
						if ( 33 <= ( (*( p))) && ( (*( p))) <= 37 ) {
							goto _st31;
						}
					} else if ( ( (*( p))) >= 10 ) {
						goto _st22;
					}
				} else if ( ( (*( p))) > 59 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st31;
						}
					} else if ( ( (*( p))) >= 61 ) {
						goto _st31;
					}
				} else {
					goto _st31;
				}
				goto _ctr0;
			}
			_ctr51:
				{
#line 158 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("public_id")), aval); }
			
#line 2424 "hpricot_scan.c"

			goto _st34;
			_ctr62:
				{
#line 158 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("public_id")), aval); }
			
#line 2431 "hpricot_scan.c"

				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2436 "hpricot_scan.c"

			goto _st34;
			_st34:
			if ( p == eof )
				goto _out34;
			p+= 1;
			st_case_34:
			if ( p == pe && p != eof )
				goto _out34;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr52;
					}
					case 32: {
						goto _ctr52;
					}
					case 33: {
						goto _ctr54;
					}
					case 39: {
						goto _ctr55;
					}
					case 62: {
						goto _ctr56;
					}
					case 91: {
						goto _ctr57;
					}
					case 95: {
						goto _ctr54;
					}
				}
				if ( ( (*( p))) < 40 ) {
					if ( ( (*( p))) > 13 ) {
						if ( 35 <= ( (*( p))) && ( (*( p))) <= 37 ) {
							goto _ctr54;
						}
					} else if ( ( (*( p))) >= 10 ) {
						goto _ctr53;
					}
				} else if ( ( (*( p))) > 59 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr54;
						}
					} else if ( ( (*( p))) >= 61 ) {
						goto _ctr54;
					}
				} else {
					goto _ctr54;
				}
				goto _ctr44;
			}
			_ctr52:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2497 "hpricot_scan.c"

			goto _st35;
			_st35:
			if ( p == eof )
				goto _out35;
			p+= 1;
			st_case_35:
			if ( p == pe && p != eof )
				goto _out35;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 9: {
						goto _st35;
					}
					case 32: {
						goto _st35;
					}
					case 34: {
						goto _st37;
					}
					case 39: {
						goto _ctr62;
					}
					case 62: {
						goto _ctr63;
					}
					case 91: {
						goto _st40;
					}
					case 95: {
						goto _st47;
					}
				}
				if ( ( (*( p))) < 40 ) {
					if ( ( (*( p))) > 13 ) {
						if ( 33 <= ( (*( p))) && ( (*( p))) <= 37 ) {
							goto _st47;
						}
					} else if ( ( (*( p))) >= 10 ) {
						goto _st36;
					}
				} else if ( ( (*( p))) > 59 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st47;
						}
					} else if ( ( (*( p))) >= 61 ) {
						goto _st47;
					}
				} else {
					goto _st47;
				}
				goto _st29;
			}
			_ctr53:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2558 "hpricot_scan.c"

			goto _st36;
			_st36:
			if ( p == eof )
				goto _out36;
			p+= 1;
			st_case_36:
			if ( p == pe && p != eof )
				goto _out36;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st36;
					}
					case 34: {
						goto _st37;
					}
					case 39: {
						goto _ctr65;
					}
					case 62: {
						goto _ctr63;
					}
					case 91: {
						goto _st40;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st36;
				}
				goto _st29;
			}
			_st37:
			if ( p == eof )
				goto _out37;
			p+= 1;
			st_case_37:
			if ( p == pe && p != eof )
				goto _out37;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr67;
					}
					case 39: {
						goto _ctr68;
					}
				}
				goto _ctr66;
			}
			_ctr66:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2617 "hpricot_scan.c"

			goto _st38;
			_st38:
			if ( p == eof )
				goto _out38;
			p+= 1;
			st_case_38:
			if ( p == pe && p != eof )
				goto _out38;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr70;
					}
					case 39: {
						goto _ctr71;
					}
				}
				goto _st38;
			}
			_ctr81:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2644 "hpricot_scan.c"

			goto _st39;
			_ctr67:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2651 "hpricot_scan.c"

				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2656 "hpricot_scan.c"

			goto _st39;
			_ctr70:
				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2663 "hpricot_scan.c"

			goto _st39;
			_st39:
			if ( p == eof )
				goto _out39;
			p+= 1;
			st_case_39:
			if ( p == pe && p != eof )
				goto _out39;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st39;
					}
					case 39: {
						goto _ctr41;
					}
					case 62: {
						goto _ctr63;
					}
					case 91: {
						goto _st40;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st39;
				}
				goto _st29;
			}
			_ctr56:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2699 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2704 "hpricot_scan.c"

				{
#line 66 "hpricot_common.rl"
				{act = 8;}}
			
#line 2709 "hpricot_scan.c"

			goto _st200;
			_ctr63:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2716 "hpricot_scan.c"

				{
#line 66 "hpricot_common.rl"
				{act = 8;}}
			
#line 2721 "hpricot_scan.c"

			goto _st200;
			_st200:
			if ( p == eof )
				goto _out200;
			p+= 1;
			st_case_200:
			if ( p == pe && p != eof )
				goto _out200;
			if ( p == eof ) {
				goto _ctr406;}
			else {
				if ( ( (*( p))) == 39 ) {
					goto _ctr41;
				}
				goto _st29;
			}
			_ctr57:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2743 "hpricot_scan.c"

			goto _st40;
			_st40:
			if ( p == eof )
				goto _out40;
			p+= 1;
			st_case_40:
			if ( p == pe && p != eof )
				goto _out40;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 39: {
						goto _ctr73;
					}
					case 93: {
						goto _st42;
					}
				}
				goto _st40;
			}
			_ctr73:
				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2770 "hpricot_scan.c"

			goto _st41;
			_st41:
			if ( p == eof )
				goto _out41;
			p+= 1;
			st_case_41:
			if ( p == pe && p != eof )
				goto _out41;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st41;
					}
					case 62: {
						goto _ctr76;
					}
					case 93: {
						goto _st27;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st41;
				}
				goto _st26;
			}
			_ctr76:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2803 "hpricot_scan.c"

				{
#line 66 "hpricot_common.rl"
				{act = 8;}}
			
#line 2808 "hpricot_scan.c"

			goto _st201;
			_st201:
			if ( p == eof )
				goto _out201;
			p+= 1;
			st_case_201:
			if ( p == pe && p != eof )
				goto _out201;
			if ( p == eof ) {
				goto _ctr406;}
			else {
				if ( ( (*( p))) == 93 ) {
					goto _st27;
				}
				goto _st26;
			}
			_st42:
			if ( p == eof )
				goto _out42;
			p+= 1;
			st_case_42:
			if ( p == pe && p != eof )
				goto _out42;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st42;
					}
					case 39: {
						goto _ctr41;
					}
					case 62: {
						goto _ctr63;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st42;
				}
				goto _st29;
			}
			_ctr68:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 2856 "hpricot_scan.c"

				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2861 "hpricot_scan.c"

			goto _st43;
			_ctr71:
				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2868 "hpricot_scan.c"

			goto _st43;
			_st43:
			if ( p == eof )
				goto _out43;
			p+= 1;
			st_case_43:
			if ( p == pe && p != eof )
				goto _out43;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st43;
					}
					case 34: {
						goto _ctr41;
					}
					case 62: {
						goto _ctr78;
					}
					case 91: {
						goto _st44;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st43;
				}
				goto _st24;
			}
			_ctr78:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2904 "hpricot_scan.c"

				{
#line 66 "hpricot_common.rl"
				{act = 8;}}
			
#line 2909 "hpricot_scan.c"

			goto _st202;
			_st202:
			if ( p == eof )
				goto _out202;
			p+= 1;
			st_case_202:
			if ( p == pe && p != eof )
				goto _out202;
			if ( p == eof ) {
				goto _ctr406;}
			else {
				if ( ( (*( p))) == 34 ) {
					goto _ctr41;
				}
				goto _st24;
			}
			_st44:
			if ( p == eof )
				goto _out44;
			p+= 1;
			st_case_44:
			if ( p == pe && p != eof )
				goto _out44;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr73;
					}
					case 93: {
						goto _st45;
					}
				}
				goto _st44;
			}
			_st45:
			if ( p == eof )
				goto _out45;
			p+= 1;
			st_case_45:
			if ( p == pe && p != eof )
				goto _out45;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st45;
					}
					case 34: {
						goto _ctr41;
					}
					case 62: {
						goto _ctr78;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st45;
				}
				goto _st24;
			}
			_ctr65:
				{
#line 159 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("system_id")), aval); }
			
#line 2977 "hpricot_scan.c"

			goto _st46;
			_st46:
			if ( p == eof )
				goto _out46;
			p+= 1;
			st_case_46:
			if ( p == pe && p != eof )
				goto _out46;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr81;
					}
					case 39: {
						goto _ctr38;
					}
					case 62: {
						goto _ctr56;
					}
					case 91: {
						goto _ctr57;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _ctr81;
				}
				goto _ctr44;
			}
			_ctr54:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 3013 "hpricot_scan.c"

			goto _st47;
			_st47:
			if ( p == eof )
				goto _out47;
			p+= 1;
			st_case_47:
			if ( p == pe && p != eof )
				goto _out47;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 9: {
						goto _st47;
					}
					case 39: {
						goto _ctr82;
					}
					case 61: {
						goto _st47;
					}
					case 95: {
						goto _st47;
					}
				}
				if ( ( (*( p))) < 40 ) {
					if ( ( (*( p))) > 33 ) {
						if ( 35 <= ( (*( p))) && ( (*( p))) <= 37 ) {
							goto _st47;
						}
					} else if ( ( (*( p))) >= 32 ) {
						goto _st47;
					}
				} else if ( ( (*( p))) > 59 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st47;
						}
					} else if ( ( (*( p))) >= 63 ) {
						goto _st47;
					}
				} else {
					goto _st47;
				}
				goto _st29;
			}
			_st48:
			if ( p == eof )
				goto _out48;
			p+= 1;
			st_case_48:
			if ( p == pe && p != eof )
				goto _out48;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 89 ) {
					goto _st49;
				}
				goto _ctr0;
			}
			_st49:
			if ( p == eof )
				goto _out49;
			p+= 1;
			st_case_49:
			if ( p == pe && p != eof )
				goto _out49;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 83 ) {
					goto _st50;
				}
				goto _ctr0;
			}
			_st50:
			if ( p == eof )
				goto _out50;
			p+= 1;
			st_case_50:
			if ( p == pe && p != eof )
				goto _out50;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 84 ) {
					goto _st51;
				}
				goto _ctr0;
			}
			_st51:
			if ( p == eof )
				goto _out51;
			p+= 1;
			st_case_51:
			if ( p == pe && p != eof )
				goto _out51;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 69 ) {
					goto _st52;
				}
				goto _ctr0;
			}
			_st52:
			if ( p == eof )
				goto _out52;
			p+= 1;
			st_case_52:
			if ( p == pe && p != eof )
				goto _out52;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 77 ) {
					goto _st21;
				}
				goto _ctr0;
			}
			_st53:
			if ( p == eof )
				goto _out53;
			p+= 1;
			st_case_53:
			if ( p == pe && p != eof )
				goto _out53;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 67 ) {
					goto _st54;
				}
				goto _ctr0;
			}
			_st54:
			if ( p == eof )
				goto _out54;
			p+= 1;
			st_case_54:
			if ( p == pe && p != eof )
				goto _out54;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 68 ) {
					goto _st55;
				}
				goto _ctr0;
			}
			_st55:
			if ( p == eof )
				goto _out55;
			p+= 1;
			st_case_55:
			if ( p == pe && p != eof )
				goto _out55;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 65 ) {
					goto _st56;
				}
				goto _ctr0;
			}
			_st56:
			if ( p == eof )
				goto _out56;
			p+= 1;
			st_case_56:
			if ( p == pe && p != eof )
				goto _out56;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 84 ) {
					goto _st57;
				}
				goto _ctr0;
			}
			_st57:
			if ( p == eof )
				goto _out57;
			p+= 1;
			st_case_57:
			if ( p == pe && p != eof )
				goto _out57;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 65 ) {
					goto _st58;
				}
				goto _ctr0;
			}
			_st58:
			if ( p == eof )
				goto _out58;
			p+= 1;
			st_case_58:
			if ( p == pe && p != eof )
				goto _out58;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				if ( ( (*( p))) == 91 ) {
					goto _ctr93;
				}
				goto _ctr0;
			}
			_st59:
			if ( p == eof )
				goto _out59;
			p+= 1;
			st_case_59:
			if ( p == pe && p != eof )
				goto _out59;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 58: {
						goto _ctr94;
					}
					case 95: {
						goto _ctr94;
					}
				}
				if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr94;
					}
				} else if ( ( (*( p))) >= 65 ) {
					goto _ctr94;
				}
				goto _ctr0;
			}
			_ctr94:
				{
#line 133 "hpricot_scan.rl"
				mark_tag = p; }
			
#line 3257 "hpricot_scan.c"

			goto _st60;
			_st60:
			if ( p == eof )
				goto _out60;
			p+= 1;
			st_case_60:
			if ( p == pe && p != eof )
				goto _out60;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr95;
					}
					case 62: {
						goto _ctr97;
					}
					case 63: {
						goto _st60;
					}
					case 95: {
						goto _st60;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( ( (*( p))) > 13 ) {
						if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
							goto _st60;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr95;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st60;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st60;
					}
				} else {
					goto _st60;
				}
				goto _ctr0;
			}
			_ctr95:
				{
#line 136 "hpricot_scan.rl"
				SET(tag, p); }
			
#line 3309 "hpricot_scan.c"

			goto _st61;
			_st61:
			if ( p == eof )
				goto _out61;
			p+= 1;
			st_case_61:
			if ( p == pe && p != eof )
				goto _out61;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st61;
					}
					case 62: {
						goto _ctr99;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st61;
				}
				goto _ctr0;
			}
			_ctr404:
				{
#line 133 "hpricot_scan.rl"
				mark_tag = p; }
			
#line 3339 "hpricot_scan.c"

			goto _st62;
			_st62:
			if ( p == eof )
				goto _out62;
			p+= 1;
			st_case_62:
			if ( p == pe && p != eof )
				goto _out62;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr100;
					}
					case 47: {
						goto _ctr102;
					}
					case 62: {
						goto _ctr103;
					}
					case 63: {
						goto _st62;
					}
					case 95: {
						goto _st62;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr100;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st62;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st62;
					}
				} else {
					goto _st62;
				}
				goto _ctr0;
			}
			_ctr100:
				{
#line 136 "hpricot_scan.rl"
				SET(tag, p); }
			
#line 3390 "hpricot_scan.c"

			goto _st63;
			_st63:
			if ( p == eof )
				goto _out63;
			p+= 1;
			st_case_63:
			if ( p == pe && p != eof )
				goto _out63;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st63;
					}
					case 47: {
						goto _st66;
					}
					case 62: {
						goto _ctr107;
					}
					case 63: {
						goto _ctr105;
					}
					case 95: {
						goto _ctr105;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st63;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr105;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr105;
					}
				} else {
					goto _ctr105;
				}
				goto _ctr0;
			}
			_ctr105:
				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 3446 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 3451 "hpricot_scan.c"

			goto _st64;
			_ctr114:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 3462 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 3472 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 3477 "hpricot_scan.c"

			goto _st64;
			_st64:
			if ( p == eof )
				goto _out64;
			p+= 1;
			st_case_64:
			if ( p == pe && p != eof )
				goto _out64;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr108;
					}
					case 47: {
						goto _ctr110;
					}
					case 61: {
						goto _ctr111;
					}
					case 62: {
						goto _ctr112;
					}
					case 63: {
						goto _st64;
					}
					case 95: {
						goto _st64;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr108;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st64;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st64;
					}
				} else {
					goto _st64;
				}
				goto _ctr39;
			}
			_ctr108:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 3531 "hpricot_scan.c"

			goto _st65;
			_ctr140:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 3541 "hpricot_scan.c"

			goto _st65;
			_ctr134:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 3548 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 3556 "hpricot_scan.c"

			goto _st65;
			_st65:
			if ( p == eof )
				goto _out65;
			p+= 1;
			st_case_65:
			if ( p == pe && p != eof )
				goto _out65;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st65;
					}
					case 47: {
						goto _ctr115;
					}
					case 61: {
						goto _st67;
					}
					case 62: {
						goto _ctr117;
					}
					case 63: {
						goto _ctr114;
					}
					case 95: {
						goto _ctr114;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st65;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr114;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr114;
					}
				} else {
					goto _ctr114;
				}
				goto _ctr39;
			}
			_ctr102:
				{
#line 136 "hpricot_scan.rl"
				SET(tag, p); }
			
#line 3610 "hpricot_scan.c"

			goto _st66;
			_ctr110:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 3617 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 3626 "hpricot_scan.c"

			goto _st66;
			_ctr115:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 3637 "hpricot_scan.c"

			goto _st66;
			_st66:
			if ( p == eof )
				goto _out66;
			p+= 1;
			st_case_66:
			if ( p == pe && p != eof )
				goto _out66;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				if ( ( (*( p))) == 62 ) {
					goto _ctr118;
				}
				goto _ctr39;
			}
			_ctr111:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 3659 "hpricot_scan.c"

			goto _st67;
			_st67:
			if ( p == eof )
				goto _out67;
			p+= 1;
			st_case_67:
			if ( p == pe && p != eof )
				goto _out67;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr120;
					}
					case 32: {
						goto _ctr120;
					}
					case 34: {
						goto _st136;
					}
					case 39: {
						goto _st137;
					}
					case 47: {
						goto _ctr124;
					}
					case 60: {
						goto _ctr39;
					}
					case 62: {
						goto _ctr117;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr121;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr120;
				}
				goto _ctr119;
			}
			_ctr119:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 3708 "hpricot_scan.c"

			goto _st68;
			_st68:
			if ( p == eof )
				goto _out68;
			p+= 1;
			st_case_68:
			if ( p == pe && p != eof )
				goto _out68;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr126;
					}
					case 32: {
						goto _ctr126;
					}
					case 47: {
						goto _ctr128;
					}
					case 60: {
						goto _ctr39;
					}
					case 62: {
						goto _ctr129;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr127;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr126;
				}
				goto _st68;
			}
			_ctr126:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 3754 "hpricot_scan.c"

			goto _st69;
			_ctr319:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 3761 "hpricot_scan.c"

				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 3766 "hpricot_scan.c"

			goto _st69;
			_ctr167:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 3773 "hpricot_scan.c"

			goto _st69;
			_st69:
			if ( p == eof )
				goto _out69;
			p+= 1;
			st_case_69:
			if ( p == pe && p != eof )
				goto _out69;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st69;
					}
					case 47: {
						goto _ctr115;
					}
					case 62: {
						goto _ctr117;
					}
					case 63: {
						goto _ctr114;
					}
					case 95: {
						goto _ctr114;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st69;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr114;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr114;
					}
				} else {
					goto _ctr114;
				}
				goto _ctr39;
			}
			_ctr127:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 3827 "hpricot_scan.c"

			goto _st70;
			_ctr155:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 3834 "hpricot_scan.c"

				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 3839 "hpricot_scan.c"

			goto _st70;
			_ctr162:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 3846 "hpricot_scan.c"

			goto _st70;
			_st70:
			if ( p == eof )
				goto _out70;
			p+= 1;
			st_case_70:
			if ( p == pe && p != eof )
				goto _out70;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr126;
					}
					case 32: {
						goto _ctr126;
					}
					case 47: {
						goto _ctr132;
					}
					case 60: {
						goto _ctr39;
					}
					case 62: {
						goto _ctr133;
					}
					case 63: {
						goto _ctr131;
					}
					case 95: {
						goto _ctr131;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr127;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr126;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr131;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr131;
					}
				} else {
					goto _ctr131;
				}
				goto _st68;
			}
			_ctr131:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 3911 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 3921 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 3926 "hpricot_scan.c"

			goto _st71;
			_ctr150:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 3933 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 3942 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 3952 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 3957 "hpricot_scan.c"

			goto _st71;
			_st71:
			if ( p == eof )
				goto _out71;
			p+= 1;
			st_case_71:
			if ( p == pe && p != eof )
				goto _out71;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr134;
					}
					case 32: {
						goto _ctr134;
					}
					case 47: {
						goto _ctr137;
					}
					case 60: {
						goto _ctr39;
					}
					case 61: {
						goto _ctr138;
					}
					case 62: {
						goto _ctr139;
					}
					case 63: {
						goto _st71;
					}
					case 95: {
						goto _st71;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr135;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr134;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st71;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st71;
					}
				} else {
					goto _st71;
				}
				goto _st68;
			}
			_ctr141:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4024 "hpricot_scan.c"

			goto _st72;
			_ctr135:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 4031 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4039 "hpricot_scan.c"

			goto _st72;
			_st72:
			if ( p == eof )
				goto _out72;
			p+= 1;
			st_case_72:
			if ( p == pe && p != eof )
				goto _out72;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr140;
					}
					case 32: {
						goto _ctr140;
					}
					case 47: {
						goto _ctr132;
					}
					case 60: {
						goto _ctr39;
					}
					case 61: {
						goto _st74;
					}
					case 62: {
						goto _ctr133;
					}
					case 63: {
						goto _ctr131;
					}
					case 95: {
						goto _ctr131;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr140;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr131;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr131;
					}
				} else {
					goto _ctr131;
				}
				goto _st68;
			}
			_ctr124:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4103 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4112 "hpricot_scan.c"

			goto _st73;
			_ctr128:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4122 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4131 "hpricot_scan.c"

			goto _st73;
			_ctr132:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4142 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4150 "hpricot_scan.c"

			goto _st73;
			_ctr137:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4160 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 4165 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4174 "hpricot_scan.c"

			goto _st73;
			_ctr147:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4181 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4189 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4198 "hpricot_scan.c"

			goto _st73;
			_ctr151:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4205 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4214 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4222 "hpricot_scan.c"

			goto _st73;
			_st73:
			if ( p == eof )
				goto _out73;
			p+= 1;
			st_case_73:
			if ( p == pe && p != eof )
				goto _out73;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr126;
					}
					case 32: {
						goto _ctr126;
					}
					case 47: {
						goto _ctr128;
					}
					case 60: {
						goto _ctr39;
					}
					case 62: {
						goto _ctr129;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr127;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr126;
				}
				goto _st68;
			}
			_ctr121:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4265 "hpricot_scan.c"

			goto _st74;
			_ctr138:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 4272 "hpricot_scan.c"

			goto _st74;
			_st74:
			if ( p == eof )
				goto _out74;
			p+= 1;
			st_case_74:
			if ( p == pe && p != eof )
				goto _out74;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr143;
					}
					case 32: {
						goto _ctr143;
					}
					case 34: {
						goto _st77;
					}
					case 39: {
						goto _st135;
					}
					case 47: {
						goto _ctr147;
					}
					case 60: {
						goto _ctr39;
					}
					case 62: {
						goto _ctr129;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr144;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr143;
				}
				goto _ctr119;
			}
			_ctr148:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4321 "hpricot_scan.c"

			goto _st75;
			_ctr143:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4328 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4336 "hpricot_scan.c"

			goto _st75;
			_st75:
			if ( p == eof )
				goto _out75;
			p+= 1;
			st_case_75:
			if ( p == pe && p != eof )
				goto _out75;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr148;
					}
					case 32: {
						goto _ctr148;
					}
					case 34: {
						goto _st136;
					}
					case 39: {
						goto _st137;
					}
					case 47: {
						goto _ctr124;
					}
					case 60: {
						goto _ctr39;
					}
					case 62: {
						goto _ctr117;
					}
					case 63: {
						goto _ctr150;
					}
					case 95: {
						goto _ctr150;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr149;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr148;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr150;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr150;
					}
				} else {
					goto _ctr150;
				}
				goto _ctr119;
			}
			_ctr149:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4403 "hpricot_scan.c"

			goto _st76;
			_ctr144:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4410 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4418 "hpricot_scan.c"

			goto _st76;
			_st76:
			if ( p == eof )
				goto _out76;
			p+= 1;
			st_case_76:
			if ( p == pe && p != eof )
				goto _out76;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr143;
					}
					case 32: {
						goto _ctr143;
					}
					case 34: {
						goto _st77;
					}
					case 39: {
						goto _st135;
					}
					case 47: {
						goto _ctr151;
					}
					case 60: {
						goto _ctr39;
					}
					case 62: {
						goto _ctr133;
					}
					case 63: {
						goto _ctr150;
					}
					case 95: {
						goto _ctr150;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr144;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr143;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr150;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr150;
					}
				} else {
					goto _ctr150;
				}
				goto _ctr119;
			}
			_st77:
			if ( p == eof )
				goto _out77;
			p+= 1;
			st_case_77:
			if ( p == pe && p != eof )
				goto _out77;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr153;
					}
					case 32: {
						goto _ctr153;
					}
					case 34: {
						goto _ctr155;
					}
					case 47: {
						goto _ctr156;
					}
					case 60: {
						goto _ctr157;
					}
					case 62: {
						goto _ctr158;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr154;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr153;
				}
				goto _ctr152;
			}
			_ctr152:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4524 "hpricot_scan.c"

			goto _st78;
			_st78:
			if ( p == eof )
				goto _out78;
			p+= 1;
			st_case_78:
			if ( p == pe && p != eof )
				goto _out78;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr160;
					}
					case 32: {
						goto _ctr160;
					}
					case 34: {
						goto _ctr162;
					}
					case 47: {
						goto _ctr163;
					}
					case 60: {
						goto _st80;
					}
					case 62: {
						goto _ctr165;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr161;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr160;
				}
				goto _st78;
			}
			_ctr323:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4570 "hpricot_scan.c"

			goto _st79;
			_ctr160:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4580 "hpricot_scan.c"

			goto _st79;
			_ctr153:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4587 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4595 "hpricot_scan.c"

			goto _st79;
			_ctr306:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4602 "hpricot_scan.c"

				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 4607 "hpricot_scan.c"

			goto _st79;
			_ctr223:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 4614 "hpricot_scan.c"

			goto _st79;
			_st79:
			if ( p == eof )
				goto _out79;
			p+= 1;
			st_case_79:
			if ( p == pe && p != eof )
				goto _out79;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st79;
					}
					case 34: {
						goto _ctr167;
					}
					case 47: {
						goto _ctr169;
					}
					case 62: {
						goto _ctr170;
					}
					case 63: {
						goto _ctr168;
					}
					case 95: {
						goto _ctr168;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st79;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr168;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr168;
					}
				} else {
					goto _ctr168;
				}
				goto _st80;
			}
			_ctr157:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4668 "hpricot_scan.c"

			goto _st80;
			_st80:
			if ( p == eof )
				goto _out80;
			p+= 1;
			st_case_80:
			if ( p == pe && p != eof )
				goto _out80;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				if ( ( (*( p))) == 34 ) {
					goto _ctr167;
				}
				goto _st80;
			}
			_ctr168:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4694 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 4704 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 4709 "hpricot_scan.c"

			goto _st81;
			_ctr324:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4716 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4725 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 4735 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 4740 "hpricot_scan.c"

			goto _st81;
			_st81:
			if ( p == eof )
				goto _out81;
			p+= 1;
			st_case_81:
			if ( p == pe && p != eof )
				goto _out81;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr171;
					}
					case 34: {
						goto _ctr167;
					}
					case 47: {
						goto _ctr173;
					}
					case 61: {
						goto _ctr174;
					}
					case 62: {
						goto _ctr175;
					}
					case 63: {
						goto _st81;
					}
					case 95: {
						goto _st81;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr171;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st81;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st81;
					}
				} else {
					goto _st81;
				}
				goto _st80;
			}
			_ctr171:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 4797 "hpricot_scan.c"

			goto _st82;
			_ctr202:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4807 "hpricot_scan.c"

			goto _st82;
			_ctr196:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 4814 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4822 "hpricot_scan.c"

			goto _st82;
			_st82:
			if ( p == eof )
				goto _out82;
			p+= 1;
			st_case_82:
			if ( p == pe && p != eof )
				goto _out82;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st82;
					}
					case 34: {
						goto _ctr167;
					}
					case 47: {
						goto _ctr169;
					}
					case 61: {
						goto _st84;
					}
					case 62: {
						goto _ctr170;
					}
					case 63: {
						goto _ctr168;
					}
					case 95: {
						goto _ctr168;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st82;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr168;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr168;
					}
				} else {
					goto _ctr168;
				}
				goto _st80;
			}
			_ctr173:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 4879 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4888 "hpricot_scan.c"

			goto _st83;
			_ctr169:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4899 "hpricot_scan.c"

			goto _st83;
			_ctr325:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4906 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4915 "hpricot_scan.c"

			goto _st83;
			_st83:
			if ( p == eof )
				goto _out83;
			p+= 1;
			st_case_83:
			if ( p == pe && p != eof )
				goto _out83;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr167;
					}
					case 62: {
						goto _ctr178;
					}
				}
				goto _st80;
			}
			_ctr158:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 4942 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 4947 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4955 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4964 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 4969 "hpricot_scan.c"

			goto _st203;
			_ctr165:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 4976 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 4984 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 4993 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 4998 "hpricot_scan.c"

			goto _st203;
			_ctr170:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 5005 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5014 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 5019 "hpricot_scan.c"

			goto _st203;
			_ctr175:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 5026 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 5031 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5040 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 5045 "hpricot_scan.c"

			goto _st203;
			_ctr178:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 5052 "hpricot_scan.c"

				{
#line 70 "hpricot_common.rl"
				{act = 12;}}
			
#line 5057 "hpricot_scan.c"

			goto _st203;
			_ctr192:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 5064 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5073 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5081 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 5086 "hpricot_scan.c"

			goto _st203;
			_ctr193:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 5093 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5098 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5107 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5115 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 5120 "hpricot_scan.c"

			goto _st203;
			_ctr201:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 5127 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5135 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 5140 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5149 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 5154 "hpricot_scan.c"

			goto _st203;
			_ctr326:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 5161 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5166 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5175 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 5180 "hpricot_scan.c"

			goto _st203;
			_st203:
			if ( p == eof )
				goto _out203;
			p+= 1;
			st_case_203:
			if ( p == pe && p != eof )
				goto _out203;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				if ( ( (*( p))) == 34 ) {
					goto _ctr167;
				}
				goto _st80;
			}
			_ctr174:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 5202 "hpricot_scan.c"

			goto _st84;
			_st84:
			if ( p == eof )
				goto _out84;
			p+= 1;
			st_case_84:
			if ( p == pe && p != eof )
				goto _out84;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr179;
					}
					case 32: {
						goto _ctr179;
					}
					case 34: {
						goto _ctr181;
					}
					case 39: {
						goto _st134;
					}
					case 47: {
						goto _ctr183;
					}
					case 60: {
						goto _st80;
					}
					case 62: {
						goto _ctr170;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr180;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr179;
				}
				goto _ctr152;
			}
			_ctr179:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5251 "hpricot_scan.c"

			goto _st85;
			_st85:
			if ( p == eof )
				goto _out85;
			p+= 1;
			st_case_85:
			if ( p == pe && p != eof )
				goto _out85;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr184;
					}
					case 32: {
						goto _ctr184;
					}
					case 34: {
						goto _ctr181;
					}
					case 39: {
						goto _st134;
					}
					case 47: {
						goto _ctr183;
					}
					case 60: {
						goto _st80;
					}
					case 62: {
						goto _ctr170;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr185;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr184;
				}
				goto _ctr152;
			}
			_ctr184:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5300 "hpricot_scan.c"

			goto _st86;
			_ctr187:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5307 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5315 "hpricot_scan.c"

			goto _st86;
			_st86:
			if ( p == eof )
				goto _out86;
			p+= 1;
			st_case_86:
			if ( p == pe && p != eof )
				goto _out86;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr184;
					}
					case 32: {
						goto _ctr184;
					}
					case 34: {
						goto _ctr181;
					}
					case 39: {
						goto _st134;
					}
					case 47: {
						goto _ctr183;
					}
					case 60: {
						goto _st80;
					}
					case 62: {
						goto _ctr170;
					}
					case 63: {
						goto _ctr186;
					}
					case 95: {
						goto _ctr186;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr185;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr184;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr186;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr186;
					}
				} else {
					goto _ctr186;
				}
				goto _ctr152;
			}
			_ctr185:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5382 "hpricot_scan.c"

			goto _st87;
			_ctr188:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5389 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5397 "hpricot_scan.c"

			goto _st87;
			_st87:
			if ( p == eof )
				goto _out87;
			p+= 1;
			st_case_87:
			if ( p == pe && p != eof )
				goto _out87;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr187;
					}
					case 32: {
						goto _ctr187;
					}
					case 34: {
						goto _ctr189;
					}
					case 39: {
						goto _st94;
					}
					case 47: {
						goto _ctr191;
					}
					case 60: {
						goto _st80;
					}
					case 62: {
						goto _ctr192;
					}
					case 63: {
						goto _ctr186;
					}
					case 95: {
						goto _ctr186;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr188;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr187;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr186;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr186;
					}
				} else {
					goto _ctr186;
				}
				goto _ctr152;
			}
			_ctr189:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 5464 "hpricot_scan.c"

			goto _st88;
			_st88:
			if ( p == eof )
				goto _out88;
			p+= 1;
			st_case_88:
			if ( p == pe && p != eof )
				goto _out88;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr153;
					}
					case 32: {
						goto _ctr153;
					}
					case 34: {
						goto _ctr155;
					}
					case 47: {
						goto _ctr191;
					}
					case 60: {
						goto _ctr157;
					}
					case 62: {
						goto _ctr193;
					}
					case 63: {
						goto _ctr186;
					}
					case 95: {
						goto _ctr186;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr154;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr153;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr186;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr186;
					}
				} else {
					goto _ctr186;
				}
				goto _ctr152;
			}
			_ctr161:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5531 "hpricot_scan.c"

			goto _st89;
			_ctr154:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5538 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5546 "hpricot_scan.c"

			goto _st89;
			_ctr209:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5553 "hpricot_scan.c"

				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 5558 "hpricot_scan.c"

			goto _st89;
			_ctr217:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 5565 "hpricot_scan.c"

			goto _st89;
			_st89:
			if ( p == eof )
				goto _out89;
			p+= 1;
			st_case_89:
			if ( p == pe && p != eof )
				goto _out89;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr160;
					}
					case 32: {
						goto _ctr160;
					}
					case 34: {
						goto _ctr162;
					}
					case 47: {
						goto _ctr195;
					}
					case 60: {
						goto _st80;
					}
					case 62: {
						goto _ctr192;
					}
					case 63: {
						goto _ctr194;
					}
					case 95: {
						goto _ctr194;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr161;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr160;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr194;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr194;
					}
				} else {
					goto _ctr194;
				}
				goto _st78;
			}
			_ctr194:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5633 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 5643 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 5648 "hpricot_scan.c"

			goto _st90;
			_ctr186:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5655 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5664 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 5674 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 5679 "hpricot_scan.c"

			goto _st90;
			_st90:
			if ( p == eof )
				goto _out90;
			p+= 1;
			st_case_90:
			if ( p == pe && p != eof )
				goto _out90;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr196;
					}
					case 32: {
						goto _ctr196;
					}
					case 34: {
						goto _ctr162;
					}
					case 47: {
						goto _ctr199;
					}
					case 60: {
						goto _st80;
					}
					case 61: {
						goto _ctr200;
					}
					case 62: {
						goto _ctr201;
					}
					case 63: {
						goto _st90;
					}
					case 95: {
						goto _st90;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr197;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr196;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st90;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st90;
					}
				} else {
					goto _st90;
				}
				goto _st78;
			}
			_ctr203:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5749 "hpricot_scan.c"

			goto _st91;
			_ctr197:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 5756 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5764 "hpricot_scan.c"

			goto _st91;
			_st91:
			if ( p == eof )
				goto _out91;
			p+= 1;
			st_case_91:
			if ( p == pe && p != eof )
				goto _out91;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr202;
					}
					case 32: {
						goto _ctr202;
					}
					case 34: {
						goto _ctr162;
					}
					case 47: {
						goto _ctr195;
					}
					case 60: {
						goto _st80;
					}
					case 61: {
						goto _st93;
					}
					case 62: {
						goto _ctr192;
					}
					case 63: {
						goto _ctr194;
					}
					case 95: {
						goto _ctr194;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr203;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr202;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr194;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr194;
					}
				} else {
					goto _ctr194;
				}
				goto _st78;
			}
			_ctr183:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5831 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5840 "hpricot_scan.c"

			goto _st92;
			_ctr163:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5850 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5859 "hpricot_scan.c"

			goto _st92;
			_ctr195:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5870 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5878 "hpricot_scan.c"

			goto _st92;
			_ctr199:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5888 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 5893 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5902 "hpricot_scan.c"

			goto _st92;
			_ctr156:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5909 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5917 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5926 "hpricot_scan.c"

			goto _st92;
			_ctr191:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5933 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 5942 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 5950 "hpricot_scan.c"

			goto _st92;
			_st92:
			if ( p == eof )
				goto _out92;
			p+= 1;
			st_case_92:
			if ( p == pe && p != eof )
				goto _out92;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr160;
					}
					case 32: {
						goto _ctr160;
					}
					case 34: {
						goto _ctr162;
					}
					case 47: {
						goto _ctr163;
					}
					case 60: {
						goto _st80;
					}
					case 62: {
						goto _ctr165;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr161;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr160;
				}
				goto _st78;
			}
			_ctr180:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 5996 "hpricot_scan.c"

			goto _st93;
			_ctr200:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 6003 "hpricot_scan.c"

			goto _st93;
			_st93:
			if ( p == eof )
				goto _out93;
			p+= 1;
			st_case_93:
			if ( p == pe && p != eof )
				goto _out93;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr187;
					}
					case 32: {
						goto _ctr187;
					}
					case 34: {
						goto _ctr189;
					}
					case 39: {
						goto _st94;
					}
					case 47: {
						goto _ctr156;
					}
					case 60: {
						goto _st80;
					}
					case 62: {
						goto _ctr165;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr188;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr187;
				}
				goto _ctr152;
			}
			_st94:
			if ( p == eof )
				goto _out94;
			p+= 1;
			st_case_94:
			if ( p == pe && p != eof )
				goto _out94;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr206;
					}
					case 32: {
						goto _ctr206;
					}
					case 34: {
						goto _ctr208;
					}
					case 39: {
						goto _ctr209;
					}
					case 47: {
						goto _ctr210;
					}
					case 60: {
						goto _ctr211;
					}
					case 62: {
						goto _ctr212;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr207;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr206;
				}
				goto _ctr205;
			}
			_ctr205:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6094 "hpricot_scan.c"

			goto _st95;
			_st95:
			if ( p == eof )
				goto _out95;
			p+= 1;
			st_case_95:
			if ( p == pe && p != eof )
				goto _out95;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr214;
					}
					case 32: {
						goto _ctr214;
					}
					case 34: {
						goto _ctr216;
					}
					case 39: {
						goto _ctr217;
					}
					case 47: {
						goto _ctr218;
					}
					case 60: {
						goto _st97;
					}
					case 62: {
						goto _ctr220;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr215;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr214;
				}
				goto _st95;
			}
			_ctr304:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6143 "hpricot_scan.c"

			goto _st96;
			_ctr214:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6153 "hpricot_scan.c"

			goto _st96;
			_ctr206:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6160 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6168 "hpricot_scan.c"

			goto _st96;
			_st96:
			if ( p == eof )
				goto _out96;
			p+= 1;
			st_case_96:
			if ( p == pe && p != eof )
				goto _out96;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st96;
					}
					case 34: {
						goto _ctr222;
					}
					case 39: {
						goto _ctr223;
					}
					case 47: {
						goto _ctr225;
					}
					case 62: {
						goto _ctr226;
					}
					case 63: {
						goto _ctr224;
					}
					case 95: {
						goto _ctr224;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st96;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr224;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr224;
					}
				} else {
					goto _ctr224;
				}
				goto _st97;
			}
			_ctr211:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6225 "hpricot_scan.c"

			goto _st97;
			_st97:
			if ( p == eof )
				goto _out97;
			p+= 1;
			st_case_97:
			if ( p == pe && p != eof )
				goto _out97;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr222;
					}
					case 39: {
						goto _ctr223;
					}
				}
				goto _st97;
			}
			_ctr318:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6252 "hpricot_scan.c"

			goto _st98;
			_ctr247:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6262 "hpricot_scan.c"

			goto _st98;
			_ctr314:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6269 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6277 "hpricot_scan.c"

			goto _st98;
			_ctr305:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6284 "hpricot_scan.c"

				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 6289 "hpricot_scan.c"

			goto _st98;
			_ctr222:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 6296 "hpricot_scan.c"

			goto _st98;
			_ctr310:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 6303 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6308 "hpricot_scan.c"

			goto _st98;
			_st98:
			if ( p == eof )
				goto _out98;
			p+= 1;
			st_case_98:
			if ( p == pe && p != eof )
				goto _out98;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st98;
					}
					case 39: {
						goto _ctr167;
					}
					case 47: {
						goto _ctr230;
					}
					case 62: {
						goto _ctr231;
					}
					case 63: {
						goto _ctr229;
					}
					case 95: {
						goto _ctr229;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st98;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr229;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr229;
					}
				} else {
					goto _ctr229;
				}
				goto _st99;
			}
			_ctr316:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6362 "hpricot_scan.c"

			goto _st99;
			_st99:
			if ( p == eof )
				goto _out99;
			p+= 1;
			st_case_99:
			if ( p == pe && p != eof )
				goto _out99;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				if ( ( (*( p))) == 39 ) {
					goto _ctr167;
				}
				goto _st99;
			}
			_ctr229:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6388 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 6398 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 6403 "hpricot_scan.c"

			goto _st100;
			_ctr320:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6410 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6419 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 6429 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 6434 "hpricot_scan.c"

			goto _st100;
			_st100:
			if ( p == eof )
				goto _out100;
			p+= 1;
			st_case_100:
			if ( p == pe && p != eof )
				goto _out100;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr232;
					}
					case 39: {
						goto _ctr167;
					}
					case 47: {
						goto _ctr234;
					}
					case 61: {
						goto _ctr235;
					}
					case 62: {
						goto _ctr236;
					}
					case 63: {
						goto _st100;
					}
					case 95: {
						goto _st100;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr232;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st100;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st100;
					}
				} else {
					goto _st100;
				}
				goto _st99;
			}
			_ctr232:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 6491 "hpricot_scan.c"

			goto _st101;
			_ctr260:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6501 "hpricot_scan.c"

			goto _st101;
			_ctr254:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 6508 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6516 "hpricot_scan.c"

			goto _st101;
			_st101:
			if ( p == eof )
				goto _out101;
			p+= 1;
			st_case_101:
			if ( p == pe && p != eof )
				goto _out101;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st101;
					}
					case 39: {
						goto _ctr167;
					}
					case 47: {
						goto _ctr230;
					}
					case 61: {
						goto _st103;
					}
					case 62: {
						goto _ctr231;
					}
					case 63: {
						goto _ctr229;
					}
					case 95: {
						goto _ctr229;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st101;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr229;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr229;
					}
				} else {
					goto _ctr229;
				}
				goto _st99;
			}
			_ctr234:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 6573 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6582 "hpricot_scan.c"

			goto _st102;
			_ctr230:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6593 "hpricot_scan.c"

			goto _st102;
			_ctr321:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6600 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6609 "hpricot_scan.c"

			goto _st102;
			_st102:
			if ( p == eof )
				goto _out102;
			p+= 1;
			st_case_102:
			if ( p == pe && p != eof )
				goto _out102;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 39: {
						goto _ctr167;
					}
					case 62: {
						goto _ctr239;
					}
				}
				goto _st99;
			}
			_ctr327:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 6636 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6641 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6649 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6658 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 6663 "hpricot_scan.c"

			goto _st204;
			_ctr250:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 6670 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6678 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6687 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 6692 "hpricot_scan.c"

			goto _st204;
			_ctr231:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 6699 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6708 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 6713 "hpricot_scan.c"

			goto _st204;
			_ctr236:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 6720 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 6725 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6734 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 6739 "hpricot_scan.c"

			goto _st204;
			_ctr239:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 6746 "hpricot_scan.c"

				{
#line 70 "hpricot_common.rl"
				{act = 12;}}
			
#line 6751 "hpricot_scan.c"

			goto _st204;
			_ctr253:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 6758 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6767 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6775 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 6780 "hpricot_scan.c"

			goto _st204;
			_ctr317:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 6787 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6792 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6801 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6809 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 6814 "hpricot_scan.c"

			goto _st204;
			_ctr259:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 6821 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6829 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 6834 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6843 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 6848 "hpricot_scan.c"

			goto _st204;
			_ctr322:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 6855 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6860 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 6869 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 6874 "hpricot_scan.c"

			goto _st204;
			_st204:
			if ( p == eof )
				goto _out204;
			p+= 1;
			st_case_204:
			if ( p == pe && p != eof )
				goto _out204;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				if ( ( (*( p))) == 39 ) {
					goto _ctr167;
				}
				goto _st99;
			}
			_ctr235:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 6896 "hpricot_scan.c"

			goto _st103;
			_st103:
			if ( p == eof )
				goto _out103;
			p+= 1;
			st_case_103:
			if ( p == pe && p != eof )
				goto _out103;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr241;
					}
					case 32: {
						goto _ctr241;
					}
					case 34: {
						goto _st130;
					}
					case 39: {
						goto _ctr244;
					}
					case 47: {
						goto _ctr245;
					}
					case 60: {
						goto _st99;
					}
					case 62: {
						goto _ctr231;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr242;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr241;
				}
				goto _ctr240;
			}
			_ctr240:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 6945 "hpricot_scan.c"

			goto _st104;
			_st104:
			if ( p == eof )
				goto _out104;
			p+= 1;
			st_case_104:
			if ( p == pe && p != eof )
				goto _out104;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr247;
					}
					case 32: {
						goto _ctr247;
					}
					case 39: {
						goto _ctr162;
					}
					case 47: {
						goto _ctr249;
					}
					case 60: {
						goto _st99;
					}
					case 62: {
						goto _ctr250;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr248;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr247;
				}
				goto _st104;
			}
			_ctr248:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 6994 "hpricot_scan.c"

			goto _st105;
			_ctr315:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7001 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7009 "hpricot_scan.c"

			goto _st105;
			_ctr272:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7016 "hpricot_scan.c"

				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 7021 "hpricot_scan.c"

			goto _st105;
			_ctr216:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 7028 "hpricot_scan.c"

			goto _st105;
			_ctr208:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 7035 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7040 "hpricot_scan.c"

			goto _st105;
			_st105:
			if ( p == eof )
				goto _out105;
			p+= 1;
			st_case_105:
			if ( p == pe && p != eof )
				goto _out105;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr247;
					}
					case 32: {
						goto _ctr247;
					}
					case 39: {
						goto _ctr162;
					}
					case 47: {
						goto _ctr252;
					}
					case 60: {
						goto _st99;
					}
					case 62: {
						goto _ctr253;
					}
					case 63: {
						goto _ctr251;
					}
					case 95: {
						goto _ctr251;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr248;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr247;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr251;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr251;
					}
				} else {
					goto _ctr251;
				}
				goto _st104;
			}
			_ctr251:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 7108 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 7118 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 7123 "hpricot_scan.c"

			goto _st106;
			_ctr270:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7130 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 7139 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 7149 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 7154 "hpricot_scan.c"

			goto _st106;
			_st106:
			if ( p == eof )
				goto _out106;
			p+= 1;
			st_case_106:
			if ( p == pe && p != eof )
				goto _out106;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr254;
					}
					case 32: {
						goto _ctr254;
					}
					case 39: {
						goto _ctr162;
					}
					case 47: {
						goto _ctr257;
					}
					case 60: {
						goto _st99;
					}
					case 61: {
						goto _ctr258;
					}
					case 62: {
						goto _ctr259;
					}
					case 63: {
						goto _st106;
					}
					case 95: {
						goto _st106;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr255;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr254;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st106;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st106;
					}
				} else {
					goto _st106;
				}
				goto _st104;
			}
			_ctr261:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7224 "hpricot_scan.c"

			goto _st107;
			_ctr255:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 7231 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7239 "hpricot_scan.c"

			goto _st107;
			_st107:
			if ( p == eof )
				goto _out107;
			p+= 1;
			st_case_107:
			if ( p == pe && p != eof )
				goto _out107;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr260;
					}
					case 32: {
						goto _ctr260;
					}
					case 39: {
						goto _ctr162;
					}
					case 47: {
						goto _ctr252;
					}
					case 60: {
						goto _st99;
					}
					case 61: {
						goto _st109;
					}
					case 62: {
						goto _ctr253;
					}
					case 63: {
						goto _ctr251;
					}
					case 95: {
						goto _ctr251;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr261;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr260;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr251;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr251;
					}
				} else {
					goto _ctr251;
				}
				goto _st104;
			}
			_ctr245:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7306 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 7315 "hpricot_scan.c"

			goto _st108;
			_ctr249:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7325 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 7334 "hpricot_scan.c"

			goto _st108;
			_ctr252:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 7345 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7353 "hpricot_scan.c"

			goto _st108;
			_ctr257:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7363 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 7368 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 7377 "hpricot_scan.c"

			goto _st108;
			_ctr267:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7384 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7392 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 7401 "hpricot_scan.c"

			goto _st108;
			_ctr271:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7408 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 7417 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7425 "hpricot_scan.c"

			goto _st108;
			_st108:
			if ( p == eof )
				goto _out108;
			p+= 1;
			st_case_108:
			if ( p == pe && p != eof )
				goto _out108;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr247;
					}
					case 32: {
						goto _ctr247;
					}
					case 39: {
						goto _ctr162;
					}
					case 47: {
						goto _ctr249;
					}
					case 60: {
						goto _st99;
					}
					case 62: {
						goto _ctr250;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr248;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr247;
				}
				goto _st104;
			}
			_ctr242:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7471 "hpricot_scan.c"

			goto _st109;
			_ctr258:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 7478 "hpricot_scan.c"

			goto _st109;
			_st109:
			if ( p == eof )
				goto _out109;
			p+= 1;
			st_case_109:
			if ( p == pe && p != eof )
				goto _out109;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr263;
					}
					case 32: {
						goto _ctr263;
					}
					case 34: {
						goto _st112;
					}
					case 39: {
						goto _ctr266;
					}
					case 47: {
						goto _ctr267;
					}
					case 60: {
						goto _st99;
					}
					case 62: {
						goto _ctr250;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr264;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr263;
				}
				goto _ctr240;
			}
			_ctr268:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7527 "hpricot_scan.c"

			goto _st110;
			_ctr263:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7534 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7542 "hpricot_scan.c"

			goto _st110;
			_st110:
			if ( p == eof )
				goto _out110;
			p+= 1;
			st_case_110:
			if ( p == pe && p != eof )
				goto _out110;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr268;
					}
					case 32: {
						goto _ctr268;
					}
					case 34: {
						goto _st130;
					}
					case 39: {
						goto _ctr244;
					}
					case 47: {
						goto _ctr245;
					}
					case 60: {
						goto _st99;
					}
					case 62: {
						goto _ctr231;
					}
					case 63: {
						goto _ctr270;
					}
					case 95: {
						goto _ctr270;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr269;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr268;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr270;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr270;
					}
				} else {
					goto _ctr270;
				}
				goto _ctr240;
			}
			_ctr269:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7609 "hpricot_scan.c"

			goto _st111;
			_ctr264:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7616 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7624 "hpricot_scan.c"

			goto _st111;
			_st111:
			if ( p == eof )
				goto _out111;
			p+= 1;
			st_case_111:
			if ( p == pe && p != eof )
				goto _out111;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr263;
					}
					case 32: {
						goto _ctr263;
					}
					case 34: {
						goto _st112;
					}
					case 39: {
						goto _ctr266;
					}
					case 47: {
						goto _ctr271;
					}
					case 60: {
						goto _st99;
					}
					case 62: {
						goto _ctr253;
					}
					case 63: {
						goto _ctr270;
					}
					case 95: {
						goto _ctr270;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr264;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr263;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr270;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr270;
					}
				} else {
					goto _ctr270;
				}
				goto _ctr240;
			}
			_st112:
			if ( p == eof )
				goto _out112;
			p+= 1;
			st_case_112:
			if ( p == pe && p != eof )
				goto _out112;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr206;
					}
					case 32: {
						goto _ctr206;
					}
					case 34: {
						goto _ctr272;
					}
					case 39: {
						goto _ctr209;
					}
					case 47: {
						goto _ctr210;
					}
					case 60: {
						goto _ctr211;
					}
					case 62: {
						goto _ctr212;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr207;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr206;
				}
				goto _ctr205;
			}
			_ctr215:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7736 "hpricot_scan.c"

			goto _st113;
			_ctr207:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7743 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7751 "hpricot_scan.c"

			goto _st113;
			_st113:
			if ( p == eof )
				goto _out113;
			p+= 1;
			st_case_113:
			if ( p == pe && p != eof )
				goto _out113;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr214;
					}
					case 32: {
						goto _ctr214;
					}
					case 34: {
						goto _ctr216;
					}
					case 39: {
						goto _ctr217;
					}
					case 47: {
						goto _ctr274;
					}
					case 60: {
						goto _st97;
					}
					case 62: {
						goto _ctr275;
					}
					case 63: {
						goto _ctr273;
					}
					case 95: {
						goto _ctr273;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr215;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr214;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr273;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr273;
					}
				} else {
					goto _ctr273;
				}
				goto _st95;
			}
			_ctr273:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 7822 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 7832 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 7837 "hpricot_scan.c"

			goto _st114;
			_ctr297:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 7844 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 7853 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 7863 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 7868 "hpricot_scan.c"

			goto _st114;
			_st114:
			if ( p == eof )
				goto _out114;
			p+= 1;
			st_case_114:
			if ( p == pe && p != eof )
				goto _out114;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr276;
					}
					case 32: {
						goto _ctr276;
					}
					case 34: {
						goto _ctr216;
					}
					case 39: {
						goto _ctr217;
					}
					case 47: {
						goto _ctr279;
					}
					case 60: {
						goto _st97;
					}
					case 61: {
						goto _ctr280;
					}
					case 62: {
						goto _ctr281;
					}
					case 63: {
						goto _st114;
					}
					case 95: {
						goto _st114;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr277;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr276;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st114;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st114;
					}
				} else {
					goto _st114;
				}
				goto _st95;
			}
			_ctr284:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 7938 "hpricot_scan.c"

			goto _st115;
			_ctr311:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7948 "hpricot_scan.c"

			goto _st115;
			_ctr276:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 7955 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 7963 "hpricot_scan.c"

			goto _st115;
			_st115:
			if ( p == eof )
				goto _out115;
			p+= 1;
			st_case_115:
			if ( p == pe && p != eof )
				goto _out115;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st115;
					}
					case 34: {
						goto _ctr222;
					}
					case 39: {
						goto _ctr223;
					}
					case 47: {
						goto _ctr225;
					}
					case 61: {
						goto _st118;
					}
					case 62: {
						goto _ctr226;
					}
					case 63: {
						goto _ctr224;
					}
					case 95: {
						goto _ctr224;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st115;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr224;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr224;
					}
				} else {
					goto _ctr224;
				}
				goto _st97;
			}
			_ctr224:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8027 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 8037 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 8042 "hpricot_scan.c"

			goto _st116;
			_ctr307:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8049 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8058 "hpricot_scan.c"

				{
#line 161 "hpricot_scan.rl"
				
				akey = Qnil;
				aval = Qnil;
				mark_akey = NULL;
				mark_aval = NULL;
			}
			
#line 8068 "hpricot_scan.c"

				{
#line 135 "hpricot_scan.rl"
				mark_akey = p; }
			
#line 8073 "hpricot_scan.c"

			goto _st116;
			_st116:
			if ( p == eof )
				goto _out116;
			p+= 1;
			st_case_116:
			if ( p == pe && p != eof )
				goto _out116;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr284;
					}
					case 34: {
						goto _ctr222;
					}
					case 39: {
						goto _ctr223;
					}
					case 47: {
						goto _ctr286;
					}
					case 61: {
						goto _ctr287;
					}
					case 62: {
						goto _ctr288;
					}
					case 63: {
						goto _st116;
					}
					case 95: {
						goto _st116;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr284;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st116;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st116;
					}
				} else {
					goto _st116;
				}
				goto _st97;
			}
			_ctr286:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 8133 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8142 "hpricot_scan.c"

			goto _st117;
			_ctr225:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8153 "hpricot_scan.c"

			goto _st117;
			_ctr308:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8160 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8169 "hpricot_scan.c"

			goto _st117;
			_st117:
			if ( p == eof )
				goto _out117;
			p+= 1;
			st_case_117:
			if ( p == pe && p != eof )
				goto _out117;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr222;
					}
					case 39: {
						goto _ctr223;
					}
					case 62: {
						goto _ctr289;
					}
				}
				goto _st97;
			}
			_ctr212:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 8199 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8204 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8212 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8221 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 8226 "hpricot_scan.c"

			goto _st205;
			_ctr220:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 8233 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8241 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8250 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 8255 "hpricot_scan.c"

			goto _st205;
			_ctr226:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 8262 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8271 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 8276 "hpricot_scan.c"

			goto _st205;
			_ctr288:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 8283 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 8288 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8297 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 8302 "hpricot_scan.c"

			goto _st205;
			_ctr289:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 8309 "hpricot_scan.c"

				{
#line 70 "hpricot_common.rl"
				{act = 12;}}
			
#line 8314 "hpricot_scan.c"

			goto _st205;
			_ctr275:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 8321 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8330 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8338 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 8343 "hpricot_scan.c"

			goto _st205;
			_ctr303:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 8350 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8355 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8364 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8372 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 8377 "hpricot_scan.c"

			goto _st205;
			_ctr281:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 8384 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8392 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 8397 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8406 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 8411 "hpricot_scan.c"

			goto _st205;
			_ctr309:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 8418 "hpricot_scan.c"

				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8423 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8432 "hpricot_scan.c"

				{
#line 68 "hpricot_common.rl"
				{act = 10;}}
			
#line 8437 "hpricot_scan.c"

			goto _st205;
			_st205:
			if ( p == eof )
				goto _out205;
			p+= 1;
			st_case_205:
			if ( p == pe && p != eof )
				goto _out205;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr222;
					}
					case 39: {
						goto _ctr223;
					}
				}
				goto _st97;
			}
			_ctr287:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 8464 "hpricot_scan.c"

			goto _st118;
			_st118:
			if ( p == eof )
				goto _out118;
			p+= 1;
			st_case_118:
			if ( p == pe && p != eof )
				goto _out118;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr290;
					}
					case 32: {
						goto _ctr290;
					}
					case 34: {
						goto _ctr292;
					}
					case 39: {
						goto _ctr293;
					}
					case 47: {
						goto _ctr294;
					}
					case 60: {
						goto _st97;
					}
					case 62: {
						goto _ctr226;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr291;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr290;
				}
				goto _ctr205;
			}
			_ctr290:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8513 "hpricot_scan.c"

			goto _st119;
			_st119:
			if ( p == eof )
				goto _out119;
			p+= 1;
			st_case_119:
			if ( p == pe && p != eof )
				goto _out119;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr295;
					}
					case 32: {
						goto _ctr295;
					}
					case 34: {
						goto _ctr292;
					}
					case 39: {
						goto _ctr293;
					}
					case 47: {
						goto _ctr294;
					}
					case 60: {
						goto _st97;
					}
					case 62: {
						goto _ctr226;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr296;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr295;
				}
				goto _ctr205;
			}
			_ctr295:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8562 "hpricot_scan.c"

			goto _st120;
			_ctr298:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8569 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8577 "hpricot_scan.c"

			goto _st120;
			_st120:
			if ( p == eof )
				goto _out120;
			p+= 1;
			st_case_120:
			if ( p == pe && p != eof )
				goto _out120;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr295;
					}
					case 32: {
						goto _ctr295;
					}
					case 34: {
						goto _ctr292;
					}
					case 39: {
						goto _ctr293;
					}
					case 47: {
						goto _ctr294;
					}
					case 60: {
						goto _st97;
					}
					case 62: {
						goto _ctr226;
					}
					case 63: {
						goto _ctr297;
					}
					case 95: {
						goto _ctr297;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr296;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr295;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr297;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr297;
					}
				} else {
					goto _ctr297;
				}
				goto _ctr205;
			}
			_ctr296:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8644 "hpricot_scan.c"

			goto _st121;
			_ctr299:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8651 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8659 "hpricot_scan.c"

			goto _st121;
			_st121:
			if ( p == eof )
				goto _out121;
			p+= 1;
			st_case_121:
			if ( p == pe && p != eof )
				goto _out121;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr298;
					}
					case 32: {
						goto _ctr298;
					}
					case 34: {
						goto _ctr300;
					}
					case 39: {
						goto _ctr301;
					}
					case 47: {
						goto _ctr302;
					}
					case 60: {
						goto _st97;
					}
					case 62: {
						goto _ctr275;
					}
					case 63: {
						goto _ctr297;
					}
					case 95: {
						goto _ctr297;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr299;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr298;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr297;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr297;
					}
				} else {
					goto _ctr297;
				}
				goto _ctr205;
			}
			_ctr300:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 8726 "hpricot_scan.c"

			goto _st122;
			_st122:
			if ( p == eof )
				goto _out122;
			p+= 1;
			st_case_122:
			if ( p == pe && p != eof )
				goto _out122;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr206;
					}
					case 32: {
						goto _ctr206;
					}
					case 34: {
						goto _ctr272;
					}
					case 39: {
						goto _ctr209;
					}
					case 47: {
						goto _ctr302;
					}
					case 60: {
						goto _ctr211;
					}
					case 62: {
						goto _ctr303;
					}
					case 63: {
						goto _ctr297;
					}
					case 95: {
						goto _ctr297;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr207;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr297;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr297;
					}
				} else {
					goto _ctr297;
				}
				goto _ctr205;
			}
			_ctr294:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8793 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8802 "hpricot_scan.c"

			goto _st123;
			_ctr218:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8812 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8821 "hpricot_scan.c"

			goto _st123;
			_ctr274:
				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8832 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8840 "hpricot_scan.c"

			goto _st123;
			_ctr279:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8850 "hpricot_scan.c"

				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 8855 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8864 "hpricot_scan.c"

			goto _st123;
			_ctr210:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8871 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8879 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8888 "hpricot_scan.c"

			goto _st123;
			_ctr302:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 8895 "hpricot_scan.c"

				{
#line 168 "hpricot_scan.rl"
				
				if (!S->xml && !NIL_P(akey))
				akey = rb_funcall(akey, s_downcase, 0);
				ATTR(akey, aval);
			}
			
#line 8904 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 8912 "hpricot_scan.c"

			goto _st123;
			_st123:
			if ( p == eof )
				goto _out123;
			p+= 1;
			st_case_123:
			if ( p == pe && p != eof )
				goto _out123;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr214;
					}
					case 32: {
						goto _ctr214;
					}
					case 34: {
						goto _ctr216;
					}
					case 39: {
						goto _ctr217;
					}
					case 47: {
						goto _ctr218;
					}
					case 60: {
						goto _st97;
					}
					case 62: {
						goto _ctr220;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr215;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr214;
				}
				goto _st95;
			}
			_ctr301:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 8961 "hpricot_scan.c"

			goto _st124;
			_st124:
			if ( p == eof )
				goto _out124;
			p+= 1;
			st_case_124:
			if ( p == pe && p != eof )
				goto _out124;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr206;
					}
					case 32: {
						goto _ctr206;
					}
					case 34: {
						goto _ctr208;
					}
					case 39: {
						goto _ctr209;
					}
					case 47: {
						goto _ctr302;
					}
					case 60: {
						goto _ctr211;
					}
					case 62: {
						goto _ctr303;
					}
					case 63: {
						goto _ctr297;
					}
					case 95: {
						goto _ctr297;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr207;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr297;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr297;
					}
				} else {
					goto _ctr297;
				}
				goto _ctr205;
			}
			_ctr292:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 9028 "hpricot_scan.c"

			goto _st125;
			_st125:
			if ( p == eof )
				goto _out125;
			p+= 1;
			st_case_125:
			if ( p == pe && p != eof )
				goto _out125;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr304;
					}
					case 34: {
						goto _ctr305;
					}
					case 39: {
						goto _ctr306;
					}
					case 47: {
						goto _ctr308;
					}
					case 62: {
						goto _ctr309;
					}
					case 63: {
						goto _ctr307;
					}
					case 95: {
						goto _ctr307;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr304;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr307;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr307;
					}
				} else {
					goto _ctr307;
				}
				goto _ctr211;
			}
			_ctr293:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 9085 "hpricot_scan.c"

			goto _st126;
			_st126:
			if ( p == eof )
				goto _out126;
			p+= 1;
			st_case_126:
			if ( p == pe && p != eof )
				goto _out126;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr304;
					}
					case 34: {
						goto _ctr310;
					}
					case 39: {
						goto _ctr306;
					}
					case 47: {
						goto _ctr308;
					}
					case 62: {
						goto _ctr309;
					}
					case 63: {
						goto _ctr307;
					}
					case 95: {
						goto _ctr307;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr304;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr307;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr307;
					}
				} else {
					goto _ctr307;
				}
				goto _ctr211;
			}
			_ctr291:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 9142 "hpricot_scan.c"

			goto _st127;
			_ctr280:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 9149 "hpricot_scan.c"

			goto _st127;
			_st127:
			if ( p == eof )
				goto _out127;
			p+= 1;
			st_case_127:
			if ( p == pe && p != eof )
				goto _out127;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr298;
					}
					case 32: {
						goto _ctr298;
					}
					case 34: {
						goto _ctr300;
					}
					case 39: {
						goto _ctr301;
					}
					case 47: {
						goto _ctr210;
					}
					case 60: {
						goto _st97;
					}
					case 62: {
						goto _ctr220;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr299;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr298;
				}
				goto _ctr205;
			}
			_ctr312:
				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 9201 "hpricot_scan.c"

			goto _st128;
			_ctr277:
				{
#line 143 "hpricot_scan.rl"
				SET(akey, p); }
			
#line 9208 "hpricot_scan.c"

				{
#line 139 "hpricot_scan.rl"
				
				if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
				else { SET(aval, p); }
			}
			
#line 9216 "hpricot_scan.c"

			goto _st128;
			_st128:
			if ( p == eof )
				goto _out128;
			p+= 1;
			st_case_128:
			if ( p == pe && p != eof )
				goto _out128;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr311;
					}
					case 32: {
						goto _ctr311;
					}
					case 34: {
						goto _ctr216;
					}
					case 39: {
						goto _ctr217;
					}
					case 47: {
						goto _ctr274;
					}
					case 60: {
						goto _st97;
					}
					case 61: {
						goto _st127;
					}
					case 62: {
						goto _ctr275;
					}
					case 63: {
						goto _ctr273;
					}
					case 95: {
						goto _ctr273;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr312;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr311;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr273;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr273;
					}
				} else {
					goto _ctr273;
				}
				goto _st95;
			}
			_ctr266:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 9286 "hpricot_scan.c"

			goto _st129;
			_st129:
			if ( p == eof )
				goto _out129;
			p+= 1;
			st_case_129:
			if ( p == pe && p != eof )
				goto _out129;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr314;
					}
					case 32: {
						goto _ctr314;
					}
					case 39: {
						goto _ctr155;
					}
					case 47: {
						goto _ctr271;
					}
					case 60: {
						goto _ctr316;
					}
					case 62: {
						goto _ctr317;
					}
					case 63: {
						goto _ctr270;
					}
					case 95: {
						goto _ctr270;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( ( (*( p))) > 10 ) {
						if ( ( (*( p))) <= 12 ) {
							goto _ctr315;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr314;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr270;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr270;
					}
				} else {
					goto _ctr270;
				}
				goto _ctr240;
			}
			_st130:
			if ( p == eof )
				goto _out130;
			p+= 1;
			st_case_130:
			if ( p == pe && p != eof )
				goto _out130;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr305;
					}
					case 39: {
						goto _ctr306;
					}
				}
				goto _ctr211;
			}
			_ctr244:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 9370 "hpricot_scan.c"

			goto _st131;
			_st131:
			if ( p == eof )
				goto _out131;
			p+= 1;
			st_case_131:
			if ( p == pe && p != eof )
				goto _out131;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr318;
					}
					case 39: {
						goto _ctr319;
					}
					case 47: {
						goto _ctr321;
					}
					case 62: {
						goto _ctr322;
					}
					case 63: {
						goto _ctr320;
					}
					case 95: {
						goto _ctr320;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr318;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr320;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr320;
					}
				} else {
					goto _ctr320;
				}
				goto _ctr316;
			}
			_ctr241:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 9424 "hpricot_scan.c"

			goto _st132;
			_st132:
			if ( p == eof )
				goto _out132;
			p+= 1;
			st_case_132:
			if ( p == pe && p != eof )
				goto _out132;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr268;
					}
					case 32: {
						goto _ctr268;
					}
					case 34: {
						goto _st130;
					}
					case 39: {
						goto _ctr244;
					}
					case 47: {
						goto _ctr245;
					}
					case 60: {
						goto _st99;
					}
					case 62: {
						goto _ctr231;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr269;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr268;
				}
				goto _ctr240;
			}
			_ctr181:
				{
#line 138 "hpricot_scan.rl"
				SET(aval, p); }
			
#line 9473 "hpricot_scan.c"

			goto _st133;
			_st133:
			if ( p == eof )
				goto _out133;
			p+= 1;
			st_case_133:
			if ( p == pe && p != eof )
				goto _out133;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr323;
					}
					case 34: {
						goto _ctr319;
					}
					case 47: {
						goto _ctr325;
					}
					case 62: {
						goto _ctr326;
					}
					case 63: {
						goto _ctr324;
					}
					case 95: {
						goto _ctr324;
					}
				}
				if ( ( (*( p))) < 45 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr323;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr324;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr324;
					}
				} else {
					goto _ctr324;
				}
				goto _ctr157;
			}
			_st134:
			if ( p == eof )
				goto _out134;
			p+= 1;
			st_case_134:
			if ( p == pe && p != eof )
				goto _out134;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr310;
					}
					case 39: {
						goto _ctr306;
					}
				}
				goto _ctr211;
			}
			_st135:
			if ( p == eof )
				goto _out135;
			p+= 1;
			st_case_135:
			if ( p == pe && p != eof )
				goto _out135;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr314;
					}
					case 32: {
						goto _ctr314;
					}
					case 39: {
						goto _ctr155;
					}
					case 47: {
						goto _ctr267;
					}
					case 60: {
						goto _ctr316;
					}
					case 62: {
						goto _ctr327;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr315;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr314;
				}
				goto _ctr240;
			}
			_st136:
			if ( p == eof )
				goto _out136;
			p+= 1;
			st_case_136:
			if ( p == pe && p != eof )
				goto _out136;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				if ( ( (*( p))) == 34 ) {
					goto _ctr319;
				}
				goto _ctr157;
			}
			_st137:
			if ( p == eof )
				goto _out137;
			p+= 1;
			st_case_137:
			if ( p == pe && p != eof )
				goto _out137;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				if ( ( (*( p))) == 39 ) {
					goto _ctr319;
				}
				goto _ctr316;
			}
			_ctr120:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 9616 "hpricot_scan.c"

			goto _st138;
			_st138:
			if ( p == eof )
				goto _out138;
			p+= 1;
			st_case_138:
			if ( p == pe && p != eof )
				goto _out138;
			if ( p == eof ) {
				goto _ctr39;}
			else {
				switch( ( (*( p))) ) {
					case 13: {
						goto _ctr148;
					}
					case 32: {
						goto _ctr148;
					}
					case 34: {
						goto _st136;
					}
					case 39: {
						goto _st137;
					}
					case 47: {
						goto _ctr124;
					}
					case 60: {
						goto _ctr39;
					}
					case 62: {
						goto _ctr117;
					}
				}
				if ( ( (*( p))) > 10 ) {
					if ( ( (*( p))) <= 12 ) {
						goto _ctr149;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr148;
				}
				goto _ctr119;
			}
			_st139:
			if ( p == eof )
				goto _out139;
			p+= 1;
			st_case_139:
			if ( p == pe && p != eof )
				goto _out139;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 58: {
						goto _ctr328;
					}
					case 95: {
						goto _ctr328;
					}
					case 120: {
						goto _ctr329;
					}
				}
				if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr328;
					}
				} else if ( ( (*( p))) >= 65 ) {
					goto _ctr328;
				}
				goto _ctr0;
			}
			_ctr328:
				{
#line 46 "hpricot_common.rl"
				TEXT_PASS(); }
			
#line 9695 "hpricot_scan.c"

			goto _st140;
			_st140:
			if ( p == eof )
				goto _out140;
			p+= 1;
			st_case_140:
			if ( p == pe && p != eof )
				goto _out140;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st206;
					}
					case 63: {
						goto _st140;
					}
					case 95: {
						goto _st140;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( ( (*( p))) > 13 ) {
						if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
							goto _st140;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _st206;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st140;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st140;
					}
				} else {
					goto _st140;
				}
				goto _ctr0;
			}
			_st206:
			if ( p == eof )
				goto _out206;
			p+= 1;
			st_case_206:
			if ( p == pe && p != eof )
				goto _out206;
			if ( p == eof ) {
				goto _ctr407;}
			else {
				if ( ( (*( p))) == 32 ) {
					goto _st206;
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st206;
				}
				goto _ctr407;
			}
			_ctr329:
				{
#line 46 "hpricot_common.rl"
				TEXT_PASS(); }
			
#line 9762 "hpricot_scan.c"

			goto _st141;
			_st141:
			if ( p == eof )
				goto _out141;
			p+= 1;
			st_case_141:
			if ( p == pe && p != eof )
				goto _out141;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st206;
					}
					case 63: {
						goto _st140;
					}
					case 95: {
						goto _st140;
					}
					case 109: {
						goto _st142;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( ( (*( p))) > 13 ) {
						if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
							goto _st140;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _st206;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st140;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st140;
					}
				} else {
					goto _st140;
				}
				goto _ctr0;
			}
			_st142:
			if ( p == eof )
				goto _out142;
			p+= 1;
			st_case_142:
			if ( p == pe && p != eof )
				goto _out142;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st206;
					}
					case 63: {
						goto _st140;
					}
					case 95: {
						goto _st140;
					}
					case 108: {
						goto _st143;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( ( (*( p))) > 13 ) {
						if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
							goto _st140;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _st206;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st140;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st140;
					}
				} else {
					goto _st140;
				}
				goto _ctr0;
			}
			_st143:
			if ( p == eof )
				goto _out143;
			p+= 1;
			st_case_143:
			if ( p == pe && p != eof )
				goto _out143;
			if ( p == eof ) {
				goto _ctr0;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr334;
					}
					case 63: {
						goto _st140;
					}
					case 95: {
						goto _st140;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( ( (*( p))) > 13 ) {
						if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
							goto _st140;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr334;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st140;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st140;
					}
				} else {
					goto _st140;
				}
				goto _ctr0;
			}
			_ctr334:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 9901 "hpricot_scan.c"

			goto _st207;
			_st207:
			if ( p == eof )
				goto _out207;
			p+= 1;
			st_case_207:
			if ( p == pe && p != eof )
				goto _out207;
			if ( p == eof ) {
				goto _ctr407;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _ctr334;
					}
					case 118: {
						goto _st144;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _ctr334;
				}
				goto _ctr407;
			}
			_st144:
			if ( p == eof )
				goto _out144;
			p+= 1;
			st_case_144:
			if ( p == pe && p != eof )
				goto _out144;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 101 ) {
					goto _st145;
				}
				goto _ctr335;
			}
			_st145:
			if ( p == eof )
				goto _out145;
			p+= 1;
			st_case_145:
			if ( p == pe && p != eof )
				goto _out145;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 114 ) {
					goto _st146;
				}
				goto _ctr335;
			}
			_st146:
			if ( p == eof )
				goto _out146;
			p+= 1;
			st_case_146:
			if ( p == pe && p != eof )
				goto _out146;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 115 ) {
					goto _st147;
				}
				goto _ctr335;
			}
			_st147:
			if ( p == eof )
				goto _out147;
			p+= 1;
			st_case_147:
			if ( p == pe && p != eof )
				goto _out147;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 105 ) {
					goto _st148;
				}
				goto _ctr335;
			}
			_st148:
			if ( p == eof )
				goto _out148;
			p+= 1;
			st_case_148:
			if ( p == pe && p != eof )
				goto _out148;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 111 ) {
					goto _st149;
				}
				goto _ctr335;
			}
			_st149:
			if ( p == eof )
				goto _out149;
			p+= 1;
			st_case_149:
			if ( p == pe && p != eof )
				goto _out149;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 110 ) {
					goto _st150;
				}
				goto _ctr335;
			}
			_st150:
			if ( p == eof )
				goto _out150;
			p+= 1;
			st_case_150:
			if ( p == pe && p != eof )
				goto _out150;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st150;
					}
					case 61: {
						goto _st151;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st150;
				}
				goto _ctr335;
			}
			_st151:
			if ( p == eof )
				goto _out151;
			p+= 1;
			st_case_151:
			if ( p == pe && p != eof )
				goto _out151;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st151;
					}
					case 34: {
						goto _st152;
					}
					case 39: {
						goto _st194;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st151;
				}
				goto _ctr335;
			}
			_st152:
			if ( p == eof )
				goto _out152;
			p+= 1;
			st_case_152:
			if ( p == pe && p != eof )
				goto _out152;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 95 ) {
					goto _ctr345;
				}
				if ( ( (*( p))) < 48 ) {
					if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
						goto _ctr345;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr345;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr345;
					}
				} else {
					goto _ctr345;
				}
				goto _ctr335;
			}
			_ctr345:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 10100 "hpricot_scan.c"

			goto _st153;
			_st153:
			if ( p == eof )
				goto _out153;
			p+= 1;
			st_case_153:
			if ( p == pe && p != eof )
				goto _out153;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr346;
					}
					case 95: {
						goto _st153;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
						goto _st153;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st153;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st153;
					}
				} else {
					goto _st153;
				}
				goto _ctr335;
			}
			_ctr346:
				{
#line 144 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("version")), aval); }
			
#line 10142 "hpricot_scan.c"

			goto _st154;
			_st154:
			if ( p == eof )
				goto _out154;
			p+= 1;
			st_case_154:
			if ( p == pe && p != eof )
				goto _out154;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st155;
					}
					case 62: {
						goto _ctr349;
					}
					case 63: {
						goto _st156;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st155;
				}
				goto _ctr335;
			}
			_st155:
			if ( p == eof )
				goto _out155;
			p+= 1;
			st_case_155:
			if ( p == pe && p != eof )
				goto _out155;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st155;
					}
					case 62: {
						goto _ctr349;
					}
					case 63: {
						goto _st156;
					}
					case 101: {
						goto _st157;
					}
					case 115: {
						goto _st170;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st155;
				}
				goto _ctr335;
			}
			_st156:
			if ( p == eof )
				goto _out156;
			p+= 1;
			st_case_156:
			if ( p == pe && p != eof )
				goto _out156;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 62 ) {
					goto _ctr349;
				}
				goto _ctr335;
			}
			_st157:
			if ( p == eof )
				goto _out157;
			p+= 1;
			st_case_157:
			if ( p == pe && p != eof )
				goto _out157;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 110 ) {
					goto _st158;
				}
				goto _ctr335;
			}
			_st158:
			if ( p == eof )
				goto _out158;
			p+= 1;
			st_case_158:
			if ( p == pe && p != eof )
				goto _out158;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 99 ) {
					goto _st159;
				}
				goto _ctr335;
			}
			_st159:
			if ( p == eof )
				goto _out159;
			p+= 1;
			st_case_159:
			if ( p == pe && p != eof )
				goto _out159;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 111 ) {
					goto _st160;
				}
				goto _ctr335;
			}
			_st160:
			if ( p == eof )
				goto _out160;
			p+= 1;
			st_case_160:
			if ( p == pe && p != eof )
				goto _out160;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 100 ) {
					goto _st161;
				}
				goto _ctr335;
			}
			_st161:
			if ( p == eof )
				goto _out161;
			p+= 1;
			st_case_161:
			if ( p == pe && p != eof )
				goto _out161;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 105 ) {
					goto _st162;
				}
				goto _ctr335;
			}
			_st162:
			if ( p == eof )
				goto _out162;
			p+= 1;
			st_case_162:
			if ( p == pe && p != eof )
				goto _out162;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 110 ) {
					goto _st163;
				}
				goto _ctr335;
			}
			_st163:
			if ( p == eof )
				goto _out163;
			p+= 1;
			st_case_163:
			if ( p == pe && p != eof )
				goto _out163;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 103 ) {
					goto _st164;
				}
				goto _ctr335;
			}
			_st164:
			if ( p == eof )
				goto _out164;
			p+= 1;
			st_case_164:
			if ( p == pe && p != eof )
				goto _out164;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st164;
					}
					case 61: {
						goto _st165;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st164;
				}
				goto _ctr335;
			}
			_st165:
			if ( p == eof )
				goto _out165;
			p+= 1;
			st_case_165:
			if ( p == pe && p != eof )
				goto _out165;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st165;
					}
					case 34: {
						goto _st166;
					}
					case 39: {
						goto _st192;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st165;
				}
				goto _ctr335;
			}
			_st166:
			if ( p == eof )
				goto _out166;
			p+= 1;
			st_case_166:
			if ( p == pe && p != eof )
				goto _out166;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr363;
					}
				} else if ( ( (*( p))) >= 65 ) {
					goto _ctr363;
				}
				goto _ctr335;
			}
			_ctr363:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 10395 "hpricot_scan.c"

			goto _st167;
			_st167:
			if ( p == eof )
				goto _out167;
			p+= 1;
			st_case_167:
			if ( p == pe && p != eof )
				goto _out167;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 34: {
						goto _ctr364;
					}
					case 95: {
						goto _st167;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
						goto _st167;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st167;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st167;
					}
				} else {
					goto _st167;
				}
				goto _ctr335;
			}
			_ctr364:
				{
#line 145 "hpricot_scan.rl"
				
#ifdef HAVE_RUBY_ENCODING_H
				if (mark_aval < p) {
					char psave = *p;
					*p = '\0';
					encoding_index = rb_enc_find_index(mark_aval);
					*p = psave;
				}
#endif
				SET(aval, p);
				ATTR(ID2SYM(rb_intern("encoding")), aval);
			}
			
#line 10448 "hpricot_scan.c"

			goto _st168;
			_st168:
			if ( p == eof )
				goto _out168;
			p+= 1;
			st_case_168:
			if ( p == pe && p != eof )
				goto _out168;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st169;
					}
					case 62: {
						goto _ctr349;
					}
					case 63: {
						goto _st156;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st169;
				}
				goto _ctr335;
			}
			_st169:
			if ( p == eof )
				goto _out169;
			p+= 1;
			st_case_169:
			if ( p == pe && p != eof )
				goto _out169;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st169;
					}
					case 62: {
						goto _ctr349;
					}
					case 63: {
						goto _st156;
					}
					case 115: {
						goto _st170;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st169;
				}
				goto _ctr335;
			}
			_st170:
			if ( p == eof )
				goto _out170;
			p+= 1;
			st_case_170:
			if ( p == pe && p != eof )
				goto _out170;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 116 ) {
					goto _st171;
				}
				goto _ctr335;
			}
			_st171:
			if ( p == eof )
				goto _out171;
			p+= 1;
			st_case_171:
			if ( p == pe && p != eof )
				goto _out171;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 97 ) {
					goto _st172;
				}
				goto _ctr335;
			}
			_st172:
			if ( p == eof )
				goto _out172;
			p+= 1;
			st_case_172:
			if ( p == pe && p != eof )
				goto _out172;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 110 ) {
					goto _st173;
				}
				goto _ctr335;
			}
			_st173:
			if ( p == eof )
				goto _out173;
			p+= 1;
			st_case_173:
			if ( p == pe && p != eof )
				goto _out173;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 100 ) {
					goto _st174;
				}
				goto _ctr335;
			}
			_st174:
			if ( p == eof )
				goto _out174;
			p+= 1;
			st_case_174:
			if ( p == pe && p != eof )
				goto _out174;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 97 ) {
					goto _st175;
				}
				goto _ctr335;
			}
			_st175:
			if ( p == eof )
				goto _out175;
			p+= 1;
			st_case_175:
			if ( p == pe && p != eof )
				goto _out175;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 108 ) {
					goto _st176;
				}
				goto _ctr335;
			}
			_st176:
			if ( p == eof )
				goto _out176;
			p+= 1;
			st_case_176:
			if ( p == pe && p != eof )
				goto _out176;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 111 ) {
					goto _st177;
				}
				goto _ctr335;
			}
			_st177:
			if ( p == eof )
				goto _out177;
			p+= 1;
			st_case_177:
			if ( p == pe && p != eof )
				goto _out177;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 110 ) {
					goto _st178;
				}
				goto _ctr335;
			}
			_st178:
			if ( p == eof )
				goto _out178;
			p+= 1;
			st_case_178:
			if ( p == pe && p != eof )
				goto _out178;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 101 ) {
					goto _st179;
				}
				goto _ctr335;
			}
			_st179:
			if ( p == eof )
				goto _out179;
			p+= 1;
			st_case_179:
			if ( p == pe && p != eof )
				goto _out179;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st179;
					}
					case 61: {
						goto _st180;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st179;
				}
				goto _ctr335;
			}
			_st180:
			if ( p == eof )
				goto _out180;
			p+= 1;
			st_case_180:
			if ( p == pe && p != eof )
				goto _out180;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st180;
					}
					case 34: {
						goto _st181;
					}
					case 39: {
						goto _st187;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st180;
				}
				goto _ctr335;
			}
			_st181:
			if ( p == eof )
				goto _out181;
			p+= 1;
			st_case_181:
			if ( p == pe && p != eof )
				goto _out181;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 110: {
						goto _ctr379;
					}
					case 121: {
						goto _ctr380;
					}
				}
				goto _ctr335;
			}
			_ctr379:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 10714 "hpricot_scan.c"

			goto _st182;
			_st182:
			if ( p == eof )
				goto _out182;
			p+= 1;
			st_case_182:
			if ( p == pe && p != eof )
				goto _out182;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 111 ) {
					goto _st183;
				}
				goto _ctr335;
			}
			_st183:
			if ( p == eof )
				goto _out183;
			p+= 1;
			st_case_183:
			if ( p == pe && p != eof )
				goto _out183;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 34 ) {
					goto _ctr382;
				}
				goto _ctr335;
			}
			_ctr382:
				{
#line 157 "hpricot_scan.rl"
				SET(aval, p); ATTR(ID2SYM(rb_intern("standalone")), aval); }
			
#line 10751 "hpricot_scan.c"

			goto _st184;
			_st184:
			if ( p == eof )
				goto _out184;
			p+= 1;
			st_case_184:
			if ( p == pe && p != eof )
				goto _out184;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 32: {
						goto _st184;
					}
					case 62: {
						goto _ctr349;
					}
					case 63: {
						goto _st156;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st184;
				}
				goto _ctr335;
			}
			_ctr380:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 10784 "hpricot_scan.c"

			goto _st185;
			_st185:
			if ( p == eof )
				goto _out185;
			p+= 1;
			st_case_185:
			if ( p == pe && p != eof )
				goto _out185;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 101 ) {
					goto _st186;
				}
				goto _ctr335;
			}
			_st186:
			if ( p == eof )
				goto _out186;
			p+= 1;
			st_case_186:
			if ( p == pe && p != eof )
				goto _out186;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 115 ) {
					goto _st183;
				}
				goto _ctr335;
			}
			_st187:
			if ( p == eof )
				goto _out187;
			p+= 1;
			st_case_187:
			if ( p == pe && p != eof )
				goto _out187;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 110: {
						goto _ctr385;
					}
					case 121: {
						goto _ctr386;
					}
				}
				goto _ctr335;
			}
			_ctr385:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 10841 "hpricot_scan.c"

			goto _st188;
			_st188:
			if ( p == eof )
				goto _out188;
			p+= 1;
			st_case_188:
			if ( p == pe && p != eof )
				goto _out188;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 111 ) {
					goto _st189;
				}
				goto _ctr335;
			}
			_st189:
			if ( p == eof )
				goto _out189;
			p+= 1;
			st_case_189:
			if ( p == pe && p != eof )
				goto _out189;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 39 ) {
					goto _ctr382;
				}
				goto _ctr335;
			}
			_ctr386:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 10878 "hpricot_scan.c"

			goto _st190;
			_st190:
			if ( p == eof )
				goto _out190;
			p+= 1;
			st_case_190:
			if ( p == pe && p != eof )
				goto _out190;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 101 ) {
					goto _st191;
				}
				goto _ctr335;
			}
			_st191:
			if ( p == eof )
				goto _out191;
			p+= 1;
			st_case_191:
			if ( p == pe && p != eof )
				goto _out191;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 115 ) {
					goto _st189;
				}
				goto _ctr335;
			}
			_st192:
			if ( p == eof )
				goto _out192;
			p+= 1;
			st_case_192:
			if ( p == pe && p != eof )
				goto _out192;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr389;
					}
				} else if ( ( (*( p))) >= 65 ) {
					goto _ctr389;
				}
				goto _ctr335;
			}
			_ctr389:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 10934 "hpricot_scan.c"

			goto _st193;
			_st193:
			if ( p == eof )
				goto _out193;
			p+= 1;
			st_case_193:
			if ( p == pe && p != eof )
				goto _out193;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 39: {
						goto _ctr364;
					}
					case 95: {
						goto _st193;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
						goto _st193;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st193;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st193;
					}
				} else {
					goto _st193;
				}
				goto _ctr335;
			}
			_st194:
			if ( p == eof )
				goto _out194;
			p+= 1;
			st_case_194:
			if ( p == pe && p != eof )
				goto _out194;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				if ( ( (*( p))) == 95 ) {
					goto _ctr391;
				}
				if ( ( (*( p))) < 48 ) {
					if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
						goto _ctr391;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr391;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr391;
					}
				} else {
					goto _ctr391;
				}
				goto _ctr335;
			}
			_ctr391:
				{
#line 134 "hpricot_scan.rl"
				mark_aval = p; }
			
#line 11006 "hpricot_scan.c"

			goto _st195;
			_st195:
			if ( p == eof )
				goto _out195;
			p+= 1;
			st_case_195:
			if ( p == pe && p != eof )
				goto _out195;
			if ( p == eof ) {
				goto _ctr335;}
			else {
				switch( ( (*( p))) ) {
					case 39: {
						goto _ctr346;
					}
					case 95: {
						goto _st195;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( 45 <= ( (*( p))) && ( (*( p))) <= 46 ) {
						goto _st195;
					}
				} else if ( ( (*( p))) > 58 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st195;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st195;
					}
				} else {
					goto _st195;
				}
				goto _ctr335;
			}
			_ctr393:
				{
#line 51 "hpricot_common.rl"
				{p = ((te))-1;
					{
#line 51 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11052 "hpricot_scan.c"

			goto _st208;
			_ctr394:
				{
#line 50 "hpricot_common.rl"
				EBLK(comment, 3); {goto _st198;}}
			
#line 11059 "hpricot_scan.c"

				{
#line 1 "-"
				{te = p+1;}}
			
#line 11064 "hpricot_scan.c"

			goto _st208;
			_ctr410:
				{
#line 51 "hpricot_common.rl"
				{te = p+1;{
#line 51 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11074 "hpricot_scan.c"

			goto _st208;
			_ctr411:
				{
#line 9 "hpricot_common.rl"
				curline += 1;}
			
#line 11081 "hpricot_scan.c"

				{
#line 51 "hpricot_common.rl"
				{te = p+1;{
#line 51 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11089 "hpricot_scan.c"

			goto _st208;
			_ctr413:
				{
#line 51 "hpricot_common.rl"
				{te = p;p = p - 1;{
#line 51 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11099 "hpricot_scan.c"

			goto _st208;
			_st208:
			if ( p == eof )
				goto _out208;
				{
#line 1 "NONE"
				{ts = 0;}}
			
#line 11108 "hpricot_scan.c"

			p+= 1;
			st_case_208:
			if ( p == pe && p != eof )
				goto _out208;
				{
#line 1 "NONE"
				{ts = p;}}
			
#line 11117 "hpricot_scan.c"

			if ( p == eof ) {
				goto _st208;}
			else {
				switch( ( (*( p))) ) {
					case 10: {
						goto _ctr411;
					}
					case 45: {
						goto _ctr412;
					}
				}
				goto _ctr410;
			}
			_ctr412:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 11136 "hpricot_scan.c"

			goto _st209;
			_st209:
			if ( p == eof )
				goto _out209;
			p+= 1;
			st_case_209:
			if ( p == pe && p != eof )
				goto _out209;
			if ( p == eof ) {
				goto _ctr413;}
			else {
				if ( ( (*( p))) == 45 ) {
					goto _st196;
				}
				goto _ctr413;
			}
			_st196:
			if ( p == eof )
				goto _out196;
			p+= 1;
			st_case_196:
			if ( p == pe && p != eof )
				goto _out196;
			if ( p == eof ) {
				goto _ctr393;}
			else {
				if ( ( (*( p))) == 62 ) {
					goto _ctr394;
				}
				goto _ctr393;
			}
			_ctr395:
				{
#line 56 "hpricot_common.rl"
				{p = ((te))-1;
					{
#line 56 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11177 "hpricot_scan.c"

			goto _st210;
			_ctr396:
				{
#line 55 "hpricot_common.rl"
				EBLK(cdata, 3); {goto _st198;}}
			
#line 11184 "hpricot_scan.c"

				{
#line 1 "-"
				{te = p+1;}}
			
#line 11189 "hpricot_scan.c"

			goto _st210;
			_ctr416:
				{
#line 56 "hpricot_common.rl"
				{te = p+1;{
#line 56 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11199 "hpricot_scan.c"

			goto _st210;
			_ctr417:
				{
#line 9 "hpricot_common.rl"
				curline += 1;}
			
#line 11206 "hpricot_scan.c"

				{
#line 56 "hpricot_common.rl"
				{te = p+1;{
#line 56 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11214 "hpricot_scan.c"

			goto _st210;
			_ctr419:
				{
#line 56 "hpricot_common.rl"
				{te = p;p = p - 1;{
#line 56 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11224 "hpricot_scan.c"

			goto _st210;
			_st210:
			if ( p == eof )
				goto _out210;
				{
#line 1 "NONE"
				{ts = 0;}}
			
#line 11233 "hpricot_scan.c"

			p+= 1;
			st_case_210:
			if ( p == pe && p != eof )
				goto _out210;
				{
#line 1 "NONE"
				{ts = p;}}
			
#line 11242 "hpricot_scan.c"

			if ( p == eof ) {
				goto _st210;}
			else {
				switch( ( (*( p))) ) {
					case 10: {
						goto _ctr417;
					}
					case 93: {
						goto _ctr418;
					}
				}
				goto _ctr416;
			}
			_ctr418:
				{
#line 1 "NONE"
				{te = p+1;}}
			
#line 11261 "hpricot_scan.c"

			goto _st211;
			_st211:
			if ( p == eof )
				goto _out211;
			p+= 1;
			st_case_211:
			if ( p == pe && p != eof )
				goto _out211;
			if ( p == eof ) {
				goto _ctr419;}
			else {
				if ( ( (*( p))) == 93 ) {
					goto _st197;
				}
				goto _ctr419;
			}
			_st197:
			if ( p == eof )
				goto _out197;
			p+= 1;
			st_case_197:
			if ( p == pe && p != eof )
				goto _out197;
			if ( p == eof ) {
				goto _ctr395;}
			else {
				if ( ( (*( p))) == 62 ) {
					goto _ctr396;
				}
				goto _ctr395;
			}
			_ctr422:
				{
#line 61 "hpricot_common.rl"
				{te = p+1;{
#line 61 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11301 "hpricot_scan.c"

			goto _st212;
			_ctr423:
				{
#line 9 "hpricot_common.rl"
				curline += 1;}
			
#line 11308 "hpricot_scan.c"

				{
#line 61 "hpricot_common.rl"
				{te = p+1;{
#line 61 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11316 "hpricot_scan.c"

			goto _st212;
			_ctr424:
				{
#line 60 "hpricot_common.rl"
				EBLK(procins, 2); {goto _st198;}}
			
#line 11323 "hpricot_scan.c"

				{
#line 1 "-"
				{te = p+1;}}
			
#line 11328 "hpricot_scan.c"

			goto _st212;
			_ctr426:
				{
#line 61 "hpricot_common.rl"
				{te = p;p = p - 1;{
#line 61 "hpricot_common.rl"
						TEXT_PASS(); }
				}}
			
#line 11338 "hpricot_scan.c"

			goto _st212;
			_st212:
			if ( p == eof )
				goto _out212;
				{
#line 1 "NONE"
				{ts = 0;}}
			
#line 11347 "hpricot_scan.c"

			p+= 1;
			st_case_212:
			if ( p == pe && p != eof )
				goto _out212;
				{
#line 1 "NONE"
				{ts = p;}}
			
#line 11356 "hpricot_scan.c"

			if ( p == eof ) {
				goto _st212;}
			else {
				switch( ( (*( p))) ) {
					case 10: {
						goto _ctr423;
					}
					case 62: {
						goto _ctr424;
					}
					case 63: {
						goto _st213;
					}
				}
				goto _ctr422;
			}
			_st213:
			if ( p == eof )
				goto _out213;
			p+= 1;
			st_case_213:
			if ( p == pe && p != eof )
				goto _out213;
			if ( p == eof ) {
				goto _ctr426;}
			else {
				if ( ( (*( p))) == 62 ) {
					goto _ctr424;
				}
				goto _ctr426;
			}
			_out198: cs = 198; goto _out; 
			_out199: cs = 199; goto _out; 
			_out0: cs = 0; goto _out; 
			_out1: cs = 1; goto _out; 
			_out2: cs = 2; goto _out; 
			_out3: cs = 3; goto _out; 
			_out4: cs = 4; goto _out; 
			_out5: cs = 5; goto _out; 
			_out6: cs = 6; goto _out; 
			_out7: cs = 7; goto _out; 
			_out8: cs = 8; goto _out; 
			_out9: cs = 9; goto _out; 
			_out10: cs = 10; goto _out; 
			_out11: cs = 11; goto _out; 
			_out12: cs = 12; goto _out; 
			_out13: cs = 13; goto _out; 
			_out14: cs = 14; goto _out; 
			_out15: cs = 15; goto _out; 
			_out16: cs = 16; goto _out; 
			_out17: cs = 17; goto _out; 
			_out18: cs = 18; goto _out; 
			_out19: cs = 19; goto _out; 
			_out20: cs = 20; goto _out; 
			_out21: cs = 21; goto _out; 
			_out22: cs = 22; goto _out; 
			_out23: cs = 23; goto _out; 
			_out24: cs = 24; goto _out; 
			_out25: cs = 25; goto _out; 
			_out26: cs = 26; goto _out; 
			_out27: cs = 27; goto _out; 
			_out28: cs = 28; goto _out; 
			_out29: cs = 29; goto _out; 
			_out30: cs = 30; goto _out; 
			_out31: cs = 31; goto _out; 
			_out32: cs = 32; goto _out; 
			_out33: cs = 33; goto _out; 
			_out34: cs = 34; goto _out; 
			_out35: cs = 35; goto _out; 
			_out36: cs = 36; goto _out; 
			_out37: cs = 37; goto _out; 
			_out38: cs = 38; goto _out; 
			_out39: cs = 39; goto _out; 
			_out200: cs = 200; goto _out; 
			_out40: cs = 40; goto _out; 
			_out41: cs = 41; goto _out; 
			_out201: cs = 201; goto _out; 
			_out42: cs = 42; goto _out; 
			_out43: cs = 43; goto _out; 
			_out202: cs = 202; goto _out; 
			_out44: cs = 44; goto _out; 
			_out45: cs = 45; goto _out; 
			_out46: cs = 46; goto _out; 
			_out47: cs = 47; goto _out; 
			_out48: cs = 48; goto _out; 
			_out49: cs = 49; goto _out; 
			_out50: cs = 50; goto _out; 
			_out51: cs = 51; goto _out; 
			_out52: cs = 52; goto _out; 
			_out53: cs = 53; goto _out; 
			_out54: cs = 54; goto _out; 
			_out55: cs = 55; goto _out; 
			_out56: cs = 56; goto _out; 
			_out57: cs = 57; goto _out; 
			_out58: cs = 58; goto _out; 
			_out59: cs = 59; goto _out; 
			_out60: cs = 60; goto _out; 
			_out61: cs = 61; goto _out; 
			_out62: cs = 62; goto _out; 
			_out63: cs = 63; goto _out; 
			_out64: cs = 64; goto _out; 
			_out65: cs = 65; goto _out; 
			_out66: cs = 66; goto _out; 
			_out67: cs = 67; goto _out; 
			_out68: cs = 68; goto _out; 
			_out69: cs = 69; goto _out; 
			_out70: cs = 70; goto _out; 
			_out71: cs = 71; goto _out; 
			_out72: cs = 72; goto _out; 
			_out73: cs = 73; goto _out; 
			_out74: cs = 74; goto _out; 
			_out75: cs = 75; goto _out; 
			_out76: cs = 76; goto _out; 
			_out77: cs = 77; goto _out; 
			_out78: cs = 78; goto _out; 
			_out79: cs = 79; goto _out; 
			_out80: cs = 80; goto _out; 
			_out81: cs = 81; goto _out; 
			_out82: cs = 82; goto _out; 
			_out83: cs = 83; goto _out; 
			_out203: cs = 203; goto _out; 
			_out84: cs = 84; goto _out; 
			_out85: cs = 85; goto _out; 
			_out86: cs = 86; goto _out; 
			_out87: cs = 87; goto _out; 
			_out88: cs = 88; goto _out; 
			_out89: cs = 89; goto _out; 
			_out90: cs = 90; goto _out; 
			_out91: cs = 91; goto _out; 
			_out92: cs = 92; goto _out; 
			_out93: cs = 93; goto _out; 
			_out94: cs = 94; goto _out; 
			_out95: cs = 95; goto _out; 
			_out96: cs = 96; goto _out; 
			_out97: cs = 97; goto _out; 
			_out98: cs = 98; goto _out; 
			_out99: cs = 99; goto _out; 
			_out100: cs = 100; goto _out; 
			_out101: cs = 101; goto _out; 
			_out102: cs = 102; goto _out; 
			_out204: cs = 204; goto _out; 
			_out103: cs = 103; goto _out; 
			_out104: cs = 104; goto _out; 
			_out105: cs = 105; goto _out; 
			_out106: cs = 106; goto _out; 
			_out107: cs = 107; goto _out; 
			_out108: cs = 108; goto _out; 
			_out109: cs = 109; goto _out; 
			_out110: cs = 110; goto _out; 
			_out111: cs = 111; goto _out; 
			_out112: cs = 112; goto _out; 
			_out113: cs = 113; goto _out; 
			_out114: cs = 114; goto _out; 
			_out115: cs = 115; goto _out; 
			_out116: cs = 116; goto _out; 
			_out117: cs = 117; goto _out; 
			_out205: cs = 205; goto _out; 
			_out118: cs = 118; goto _out; 
			_out119: cs = 119; goto _out; 
			_out120: cs = 120; goto _out; 
			_out121: cs = 121; goto _out; 
			_out122: cs = 122; goto _out; 
			_out123: cs = 123; goto _out; 
			_out124: cs = 124; goto _out; 
			_out125: cs = 125; goto _out; 
			_out126: cs = 126; goto _out; 
			_out127: cs = 127; goto _out; 
			_out128: cs = 128; goto _out; 
			_out129: cs = 129; goto _out; 
			_out130: cs = 130; goto _out; 
			_out131: cs = 131; goto _out; 
			_out132: cs = 132; goto _out; 
			_out133: cs = 133; goto _out; 
			_out134: cs = 134; goto _out; 
			_out135: cs = 135; goto _out; 
			_out136: cs = 136; goto _out; 
			_out137: cs = 137; goto _out; 
			_out138: cs = 138; goto _out; 
			_out139: cs = 139; goto _out; 
			_out140: cs = 140; goto _out; 
			_out206: cs = 206; goto _out; 
			_out141: cs = 141; goto _out; 
			_out142: cs = 142; goto _out; 
			_out143: cs = 143; goto _out; 
			_out207: cs = 207; goto _out; 
			_out144: cs = 144; goto _out; 
			_out145: cs = 145; goto _out; 
			_out146: cs = 146; goto _out; 
			_out147: cs = 147; goto _out; 
			_out148: cs = 148; goto _out; 
			_out149: cs = 149; goto _out; 
			_out150: cs = 150; goto _out; 
			_out151: cs = 151; goto _out; 
			_out152: cs = 152; goto _out; 
			_out153: cs = 153; goto _out; 
			_out154: cs = 154; goto _out; 
			_out155: cs = 155; goto _out; 
			_out156: cs = 156; goto _out; 
			_out157: cs = 157; goto _out; 
			_out158: cs = 158; goto _out; 
			_out159: cs = 159; goto _out; 
			_out160: cs = 160; goto _out; 
			_out161: cs = 161; goto _out; 
			_out162: cs = 162; goto _out; 
			_out163: cs = 163; goto _out; 
			_out164: cs = 164; goto _out; 
			_out165: cs = 165; goto _out; 
			_out166: cs = 166; goto _out; 
			_out167: cs = 167; goto _out; 
			_out168: cs = 168; goto _out; 
			_out169: cs = 169; goto _out; 
			_out170: cs = 170; goto _out; 
			_out171: cs = 171; goto _out; 
			_out172: cs = 172; goto _out; 
			_out173: cs = 173; goto _out; 
			_out174: cs = 174; goto _out; 
			_out175: cs = 175; goto _out; 
			_out176: cs = 176; goto _out; 
			_out177: cs = 177; goto _out; 
			_out178: cs = 178; goto _out; 
			_out179: cs = 179; goto _out; 
			_out180: cs = 180; goto _out; 
			_out181: cs = 181; goto _out; 
			_out182: cs = 182; goto _out; 
			_out183: cs = 183; goto _out; 
			_out184: cs = 184; goto _out; 
			_out185: cs = 185; goto _out; 
			_out186: cs = 186; goto _out; 
			_out187: cs = 187; goto _out; 
			_out188: cs = 188; goto _out; 
			_out189: cs = 189; goto _out; 
			_out190: cs = 190; goto _out; 
			_out191: cs = 191; goto _out; 
			_out192: cs = 192; goto _out; 
			_out193: cs = 193; goto _out; 
			_out194: cs = 194; goto _out; 
			_out195: cs = 195; goto _out; 
			_out208: cs = 208; goto _out; 
			_out209: cs = 209; goto _out; 
			_out196: cs = 196; goto _out; 
			_out210: cs = 210; goto _out; 
			_out211: cs = 211; goto _out; 
			_out197: cs = 197; goto _out; 
			_out212: cs = 212; goto _out; 
			_out213: cs = 213; goto _out; 
			_out: {}
		}
		
#line 601 "hpricot_scan.rl"

		
		if (cs == hpricot_scan_error) {
			if (buf != NULL)
				free(buf);
			if (!NIL_P(tag))
				{
				rb_raise(rb_eHpricotParseError, "parse error on element <%s>, starting on line %d.\n" NO_WAY_SERIOUSLY, RSTRING_PTR(tag), curline);
			}
			else
				{
				rb_raise(rb_eHpricotParseError, "parse error on line %d.\n" NO_WAY_SERIOUSLY, curline);
			}
		}
		
		if (done && ele_open)
			{
			ele_open = 0;
			if (ts > 0) {
				mark_tag = ts;
				ts = 0;
				text = 1;
			}
		}
		
		if (ts == 0)
			{
			have = 0;
			/* text nodes have no ts because each byte is parsed alone */
			if (mark_tag != NULL && text == 1)
				{
				if (done)
					{
					if (mark_tag < p-1)
						{
						CAT(tag, p-1);
						ELE(text);
					}
				}
				else
					{
					CAT(tag, p);
				}
			}
			if (io)
				mark_tag = buf;
			else
				mark_tag = RSTRING_PTR(port);
		}
		else if (io)
			{
			have = pe - ts;
			memmove(buf, ts, have);
			SLIDE(tag);
			SLIDE(akey);
			SLIDE(aval);
			te = buf + (te - ts);
			ts = buf;
		}
	}
	
	if (buf != NULL)
		free(buf);
	
	if (S != NULL)
		{
		VALUE doc = S->doc;
		rb_gc_unregister_address(&S->doc);
		free(S);
		return doc;
	}
	
	return Qnil;
}

void hstruct_mark(void* ptr) {
	struct hpricot_struct* st = (struct hpricot_struct*)ptr;
	int i;
	
	/* it's likely to hit GC when allocating st->ptr.
	* that should be checked to avoid segfault.
	* and simply ignore it.
	*/
	if (st->ptr) {
		for(i = 0; i < st->len; i++) {
			rb_gc_mark(st->ptr[i]);
		}
	}
}

void hstruct_free(void* ptr) {
	struct hpricot_struct* st = (struct hpricot_struct*)ptr;
	
	free(st->ptr);
	free(st);
}

static VALUE
alloc_hpricot_struct8(VALUE klass)
{
	VALUE obj;
	struct hpricot_struct* st;
	
	obj = Data_Make_Struct(klass, struct hpricot_struct, hstruct_mark, hstruct_free, st);
	
	st->len = 8;
	st->ptr = ALLOC_N(VALUE, 8);
	
	rb_mem_clear(st->ptr, 8);
	
	return obj;
}

static VALUE
alloc_hpricot_struct2(VALUE klass)
{
	VALUE obj;
	struct hpricot_struct* st;
	
	obj = Data_Make_Struct(klass, struct hpricot_struct, hstruct_mark, hstruct_free, st);
	
	st->len = 2;
	st->ptr = ALLOC_N(VALUE, 2);
	
	rb_mem_clear(st->ptr, 2);
	
	return obj;
}

static VALUE
alloc_hpricot_struct3(VALUE klass)
{
	VALUE obj;
	struct hpricot_struct* st;
	
	obj = Data_Make_Struct(klass, struct hpricot_struct, hstruct_mark, hstruct_free, st);
	
	st->len = 3;
	st->ptr = ALLOC_N(VALUE, 3);
	
	rb_mem_clear(st->ptr, 3);
	
	return obj;
}

static VALUE hpricot_struct_ref0(VALUE obj) {return H_ELE_GET(obj, 0);}
static VALUE hpricot_struct_ref1(VALUE obj) {return H_ELE_GET(obj, 1);}
static VALUE hpricot_struct_ref2(VALUE obj) {return H_ELE_GET(obj, 2);}
static VALUE hpricot_struct_ref3(VALUE obj) {return H_ELE_GET(obj, 3);}
static VALUE hpricot_struct_ref4(VALUE obj) {return H_ELE_GET(obj, 4);}
static VALUE hpricot_struct_ref5(VALUE obj) {return H_ELE_GET(obj, 5);}
static VALUE hpricot_struct_ref6(VALUE obj) {return H_ELE_GET(obj, 6);}
static VALUE hpricot_struct_ref7(VALUE obj) {return H_ELE_GET(obj, 7);}
static VALUE hpricot_struct_ref8(VALUE obj) {return H_ELE_GET(obj, 8);}
static VALUE hpricot_struct_ref9(VALUE obj) {return H_ELE_GET(obj, 9);}

static VALUE (*ref_func[10])() = {
	hpricot_struct_ref0,
	hpricot_struct_ref1,
	hpricot_struct_ref2,
	hpricot_struct_ref3,
	hpricot_struct_ref4,
	hpricot_struct_ref5,
	hpricot_struct_ref6,
	hpricot_struct_ref7,
	hpricot_struct_ref8,
	hpricot_struct_ref9,
};

static VALUE hpricot_struct_set0(VALUE obj, VALUE val) {return H_ELE_SET(obj, 0, val);}
static VALUE hpricot_struct_set1(VALUE obj, VALUE val) {return H_ELE_SET(obj, 1, val);}
static VALUE hpricot_struct_set2(VALUE obj, VALUE val) {return H_ELE_SET(obj, 2, val);}
static VALUE hpricot_struct_set3(VALUE obj, VALUE val) {return H_ELE_SET(obj, 3, val);}
static VALUE hpricot_struct_set4(VALUE obj, VALUE val) {return H_ELE_SET(obj, 4, val);}
static VALUE hpricot_struct_set5(VALUE obj, VALUE val) {return H_ELE_SET(obj, 5, val);}
static VALUE hpricot_struct_set6(VALUE obj, VALUE val) {return H_ELE_SET(obj, 6, val);}
static VALUE hpricot_struct_set7(VALUE obj, VALUE val) {return H_ELE_SET(obj, 7, val);}
static VALUE hpricot_struct_set8(VALUE obj, VALUE val) {return H_ELE_SET(obj, 8, val);}
static VALUE hpricot_struct_set9(VALUE obj, VALUE val) {return H_ELE_SET(obj, 9, val);}

static VALUE (*set_func[10])() = {
	hpricot_struct_set0,
	hpricot_struct_set1,
	hpricot_struct_set2,
	hpricot_struct_set3,
	hpricot_struct_set4,
	hpricot_struct_set5,
	hpricot_struct_set6,
	hpricot_struct_set7,
	hpricot_struct_set8,
	hpricot_struct_set9,
};

static VALUE
make_hpricot_struct(VALUE members, VALUE (*alloc)(VALUE klass))
{
	int i = 0;
	char attr_set[128];
	
	VALUE klass = rb_class_new(rb_cObject);
	rb_define_alloc_func(klass, alloc);
	
	int len = RARRAY_LEN(members);
	assert(len < 10);
	
	for (i = 0; i < len; i++) {
		ID id = SYM2ID(rb_ary_entry(members, i));
		const char* name = rb_id2name(id);
		int len = strlen(name);
		
		memcpy(attr_set, name, strlen(name));
		attr_set[len] = '=';
		attr_set[len+1] = 0;
		
		rb_define_method(klass, name, ref_func[i], 0);
		rb_define_method(klass, attr_set, set_func[i], 1);
	}
	return klass;
}

void Init_hpricot_scan()
{
	VALUE structElem, structAttr, structBasic;
	
	s_ElementContent = rb_intern("ElementContent");
	symAllow = ID2SYM(rb_intern("allow"));
	symDeny = ID2SYM(rb_intern("deny"));
	s_downcase = rb_intern("downcase");
	s_new = rb_intern("new");
	s_parent = rb_intern("parent");
	s_read = rb_intern("read");
	s_to_str = rb_intern("to_str");
	sym_xmldecl = ID2SYM(rb_intern("xmldecl"));
	sym_doctype = ID2SYM(rb_intern("doctype"));
	sym_procins = ID2SYM(rb_intern("procins"));
	sym_stag = ID2SYM(rb_intern("stag"));
	sym_etag = ID2SYM(rb_intern("etag"));
	sym_emptytag = ID2SYM(rb_intern("emptytag"));
	sym_allowed = ID2SYM(rb_intern("allowed"));
	sym_children = ID2SYM(rb_intern("children"));
	sym_comment = ID2SYM(rb_intern("comment"));
	sym_cdata = ID2SYM(rb_intern("cdata"));
	sym_name = ID2SYM(rb_intern("name"));
	sym_parent = ID2SYM(rb_intern("parent"));
	sym_raw_attributes = ID2SYM(rb_intern("raw_attributes"));
	sym_raw_string = ID2SYM(rb_intern("raw_string"));
	sym_tagno = ID2SYM(rb_intern("tagno"));
	sym_text = ID2SYM(rb_intern("text"));
	sym_EMPTY = ID2SYM(rb_intern("EMPTY"));
	sym_CDATA = ID2SYM(rb_intern("CDATA"));
	
	mHpricot = rb_define_module("Hpricot");
	rb_define_attr(rb_singleton_class(mHpricot), "buffer_size", 1, 1);
	rb_define_singleton_method(mHpricot, "scan", hpricot_scan, -1);
	rb_define_singleton_method(mHpricot, "css", hpricot_css, 3);
	rb_eHpricotParseError = rb_define_class_under(mHpricot, "ParseError", rb_eStandardError);
	
	structElem = make_hpricot_struct(rb_ary_new3(8, sym_name, sym_parent,
	sym_raw_attributes, sym_etag, sym_raw_string, sym_allowed,
	sym_tagno, sym_children), alloc_hpricot_struct8);
	structAttr = make_hpricot_struct(
	rb_ary_new3(3, sym_name, sym_parent, sym_raw_attributes),
	alloc_hpricot_struct3);
	structBasic = make_hpricot_struct(
	rb_ary_new3(2, sym_name, sym_parent),
	alloc_hpricot_struct2);
	
	cDoc = rb_define_class_under(mHpricot, "Doc", structElem);
	cCData = rb_define_class_under(mHpricot, "CData", structBasic);
	rb_define_method(cCData, "content", hpricot_ele_get_name, 0);
	rb_define_method(cCData, "content=", hpricot_ele_set_name, 1);
	cComment = rb_define_class_under(mHpricot, "Comment", structBasic);
	rb_define_method(cComment, "content", hpricot_ele_get_name, 0);
	rb_define_method(cComment, "content=", hpricot_ele_set_name, 1);
	cDocType = rb_define_class_under(mHpricot, "DocType", structAttr);
	rb_define_method(cDocType, "raw_string", hpricot_ele_get_name, 0);
	rb_define_method(cDocType, "clear_raw", hpricot_ele_clear_name, 0);
	rb_define_method(cDocType, "target", hpricot_ele_get_target, 0);
	rb_define_method(cDocType, "target=", hpricot_ele_set_target, 1);
	rb_define_method(cDocType, "public_id", hpricot_ele_get_public_id, 0);
	rb_define_method(cDocType, "public_id=", hpricot_ele_set_public_id, 1);
	rb_define_method(cDocType, "system_id", hpricot_ele_get_system_id, 0);
	rb_define_method(cDocType, "system_id=", hpricot_ele_set_system_id, 1);
	cElem = rb_define_class_under(mHpricot, "Elem", structElem);
	rb_define_method(cElem, "clear_raw", hpricot_ele_clear_raw, 0);
	cBogusETag = rb_define_class_under(mHpricot, "BogusETag", structAttr);
	rb_define_method(cBogusETag, "raw_string", hpricot_ele_get_attr, 0);
	rb_define_method(cBogusETag, "clear_raw", hpricot_ele_clear_attr, 0);
	cText = rb_define_class_under(mHpricot, "Text", structBasic);
	rb_define_method(cText, "raw_string", hpricot_ele_get_name, 0);
	rb_define_method(cText, "clear_raw", hpricot_ele_clear_name, 0);
	rb_define_method(cText, "content", hpricot_ele_get_name, 0);
	rb_define_method(cText, "content=", hpricot_ele_set_name, 1);
	cXMLDecl = rb_define_class_under(mHpricot, "XMLDecl", structAttr);
	rb_define_method(cXMLDecl, "raw_string", hpricot_ele_get_name, 0);
	rb_define_method(cXMLDecl, "clear_raw", hpricot_ele_clear_name, 0);
	rb_define_method(cXMLDecl, "encoding", hpricot_ele_get_encoding, 0);
	rb_define_method(cXMLDecl, "encoding=", hpricot_ele_set_encoding, 1);
	rb_define_method(cXMLDecl, "standalone", hpricot_ele_get_standalone, 0);
	rb_define_method(cXMLDecl, "standalone=", hpricot_ele_set_standalone, 1);
	rb_define_method(cXMLDecl, "version", hpricot_ele_get_version, 0);
	rb_define_method(cXMLDecl, "version=", hpricot_ele_set_version, 1);
	cProcIns = rb_define_class_under(mHpricot, "ProcIns", structAttr);
	rb_define_method(cProcIns, "target", hpricot_ele_get_name, 0);
	rb_define_method(cProcIns, "target=", hpricot_ele_set_name, 1);
	rb_define_method(cProcIns, "content", hpricot_ele_get_attr, 0);
	rb_define_method(cProcIns, "content=", hpricot_ele_set_attr, 1);
	
	rb_const_set(mHpricot, rb_intern("ProcInsParse"),
	reProcInsParse = rb_eval_string("/\\A<\\?(\\S+)\\s+(.+)/m"));
}
