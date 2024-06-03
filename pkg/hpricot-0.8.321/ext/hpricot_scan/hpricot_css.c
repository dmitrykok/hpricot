#line 1 "hpricot_css.rl"
/*
* hpricot_css.rl
* ragel -C hpricot_css.rl -o hpricot_css.c
*
* Copyright (C) 2008 why the lucky stiff
*/
#include <ruby.h>

#define FILTER(id) \
rb_funcall2(mod, rb_intern("" # id), fargs, fvals); \
rb_ary_clear(tmpt); \
fargs = 1
#define FILTERAUTO() \
char filt[10]; \
sprintf(filt, "%.*s", te - ts, ts); \
rb_funcall2(mod, rb_intern(filt), fargs, fvals); \
rb_ary_clear(tmpt); \
fargs = 1
#ifdef HAVE_RUBY_ENCODING_H
#define STRNEW(a, len)  rb_external_str_new((a), (len))
#else
#define STRNEW(a, len)  rb_str_new((a), (len))
#endif
#define PUSH(aps, ape) rb_ary_push(tmpt, fvals[fargs++] = STRNEW(aps, ape - aps))
#define P(id) printf(id ": %.*s\n", te - ts, ts);


#line 28 "hpricot_css.c"
static const int hpricot_css_start = 87;
static const int hpricot_css_error = 0;

static const int hpricot_css_en_main = 87;


#line 92 "hpricot_css.rl"


VALUE hpricot_css(VALUE self, VALUE mod, VALUE str, VALUE node)
{
	int cs, act, eof;
	char *p, *pe, *ts, *te, *aps, *ape, *aps2, *ape2;
	
	int fargs = 1;
	VALUE fvals[6];
	VALUE focus = rb_ary_new3(1, node);
	VALUE tmpt = rb_ary_new();
	rb_gc_register_address(&focus);
	rb_gc_register_address(&tmpt);
	fvals[0] = focus;
	
	if (TYPE(str) != T_STRING)
		rb_raise(rb_eArgError, "bad CSS selector, String only please.");
	
	StringValue(str);
	p = RSTRING_PTR(str);
	pe = p + RSTRING_LEN(str);
	

#line 56 "hpricot_css.c"
	{
		cs = (int)hpricot_css_start;
		ts = 0;
		te = 0;
		act = 0;
	}
	
#line 114 "hpricot_css.rl"


#line 64 "hpricot_css.c"
{
		switch ( cs ) {
			case 87:
			goto st_case_87;
			case 0:
			goto st_case_0;
			case 1:
			goto st_case_1;
			case 88:
			goto st_case_88;
			case 2:
			goto st_case_2;
			case 3:
			goto st_case_3;
			case 4:
			goto st_case_4;
			case 5:
			goto st_case_5;
			case 89:
			goto st_case_89;
			case 6:
			goto st_case_6;
			case 90:
			goto st_case_90;
			case 7:
			goto st_case_7;
			case 8:
			goto st_case_8;
			case 91:
			goto st_case_91;
			case 9:
			goto st_case_9;
			case 10:
			goto st_case_10;
			case 11:
			goto st_case_11;
			case 12:
			goto st_case_12;
			case 92:
			goto st_case_92;
			case 93:
			goto st_case_93;
			case 13:
			goto st_case_13;
			case 14:
			goto st_case_14;
			case 94:
			goto st_case_94;
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
			case 95:
			goto st_case_95;
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
			case 40:
			goto st_case_40;
			case 41:
			goto st_case_41;
			case 96:
			goto st_case_96;
			case 97:
			goto st_case_97;
			case 42:
			goto st_case_42;
			case 43:
			goto st_case_43;
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
			case 118:
			goto st_case_118;
			case 119:
			goto st_case_119;
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
			case 120:
			goto st_case_120;
			case 60:
			goto st_case_60;
			case 61:
			goto st_case_61;
			case 62:
			goto st_case_62;
			case 121:
			goto st_case_121;
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
			case 84:
			goto st_case_84;
			case 85:
			goto st_case_85;
			case 86:
			goto st_case_86;
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
		}
		_ctr1:
			{
#line 1 "NONE"
			{switch( act ) {
					case 0:  {
						{
#line 1 "NONE"
							{goto _st0;}}
						break; 
					}
					case 1:  {
						p = ((te))-1;
						{
#line 77 "hpricot_css.rl"
							FILTER(ID); }
						break; 
					}
					case 2:  {
						p = ((te))-1;
						{
#line 78 "hpricot_css.rl"
							FILTER(CLASS); }
						break; 
					}
					case 5:  {
						p = ((te))-1;
						{
#line 81 "hpricot_css.rl"
							FILTER(TAG); }
						break; 
					}
					case 7:  {
						p = ((te))-1;
						{
#line 83 "hpricot_css.rl"
							FILTER(CHILD); }
						break; 
					}
					case 8:  {
						p = ((te))-1;
						{
#line 84 "hpricot_css.rl"
							FILTER(POS); }
						break; 
					}
					case 9:  {
						p = ((te))-1;
						{
#line 85 "hpricot_css.rl"
							FILTER(PSUEDO); }
						break; 
					}
				}}
		}
		
#line 376 "hpricot_css.c"

		goto _st87;
		_ctr5:
			{
#line 1 "-"
			{p = ((te))-1;
			}}
		
#line 384 "hpricot_css.c"

		goto _st87;
		_ctr44:
			{
#line 85 "hpricot_css.rl"
			{p = ((te))-1;
				{
#line 85 "hpricot_css.rl"
					FILTER(PSUEDO); }
			}}
		
#line 395 "hpricot_css.c"

		goto _st87;
		_ctr49:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 404 "hpricot_css.c"

			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 412 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{te = p+1;{
#line 85 "hpricot_css.rl"
					FILTER(PSUEDO); }
			}}
		
#line 420 "hpricot_css.c"

		goto _st87;
		_ctr51:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 430 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{te = p+1;{
#line 85 "hpricot_css.rl"
					FILTER(PSUEDO); }
			}}
		
#line 438 "hpricot_css.c"

		goto _st87;
		_ctr65:
			{
#line 84 "hpricot_css.rl"
			{p = ((te))-1;
				{
#line 84 "hpricot_css.rl"
					FILTER(POS); }
			}}
		
#line 449 "hpricot_css.c"

		goto _st87;
		_ctr67:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 459 "hpricot_css.c"

			{
#line 84 "hpricot_css.rl"
			{te = p+1;{
#line 84 "hpricot_css.rl"
					FILTER(POS); }
			}}
		
#line 467 "hpricot_css.c"

		goto _st87;
		_ctr69:
			{
#line 83 "hpricot_css.rl"
			{p = ((te))-1;
				{
#line 83 "hpricot_css.rl"
					FILTER(CHILD); }
			}}
		
#line 478 "hpricot_css.c"

		goto _st87;
		_ctr70:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 487 "hpricot_css.c"

			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 495 "hpricot_css.c"

			{
#line 83 "hpricot_css.rl"
			{te = p+1;{
#line 83 "hpricot_css.rl"
					FILTER(CHILD); }
			}}
		
#line 503 "hpricot_css.c"

		goto _st87;
		_ctr74:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 513 "hpricot_css.c"

			{
#line 83 "hpricot_css.rl"
			{te = p+1;{
#line 83 "hpricot_css.rl"
					FILTER(CHILD); }
			}}
		
#line 521 "hpricot_css.c"

		goto _st87;
		_ctr106:
			{
#line 80 "hpricot_css.rl"
			{te = p+1;{
#line 80 "hpricot_css.rl"
					FILTER(ATTR); }
			}}
		
#line 531 "hpricot_css.c"

		goto _st87;
		_ctr111:
			{
#line 80 "hpricot_css.rl"
			{p = ((te))-1;
				{
#line 80 "hpricot_css.rl"
					FILTER(ATTR); }
			}}
		
#line 542 "hpricot_css.c"

		goto _st87;
		_ctr150:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 552 "hpricot_css.c"

			{
#line 79 "hpricot_css.rl"
			{te = p+1;{
#line 79 "hpricot_css.rl"
					FILTER(NAME); }
			}}
		
#line 560 "hpricot_css.c"

		goto _st87;
		_ctr159:
			{
#line 87 "hpricot_css.rl"
			{te = p+1;{
#line 87 "hpricot_css.rl"
					FILTERAUTO(); }
			}}
		
#line 570 "hpricot_css.c"

		goto _st87;
		_ctr164:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 580 "hpricot_css.c"

			{
#line 81 "hpricot_css.rl"
			{te = p;p = p - 1;{
#line 81 "hpricot_css.rl"
					FILTER(TAG); }
			}}
		
#line 588 "hpricot_css.c"

		goto _st87;
		_ctr168:
			{
#line 1 "-"
			{te = p;p = p - 1;}}
		
#line 595 "hpricot_css.c"

		goto _st87;
		_ctr169:
			{
#line 86 "hpricot_css.rl"
			{te = p;p = p - 1;{
#line 86 "hpricot_css.rl"
					focus = rb_ary_new3(1, node); }
			}}
		
#line 605 "hpricot_css.c"

		goto _st87;
		_ctr170:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 615 "hpricot_css.c"

			{
#line 77 "hpricot_css.rl"
			{te = p;p = p - 1;{
#line 77 "hpricot_css.rl"
					FILTER(ID); }
			}}
		
#line 623 "hpricot_css.c"

		goto _st87;
		_ctr174:
			{
#line 82 "hpricot_css.rl"
			{te = p;p = p - 1;{
#line 82 "hpricot_css.rl"
					FILTER(MOD); }
			}}
		
#line 633 "hpricot_css.c"

		goto _st87;
		_ctr177:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 643 "hpricot_css.c"

			{
#line 78 "hpricot_css.rl"
			{te = p;p = p - 1;{
#line 78 "hpricot_css.rl"
					FILTER(CLASS); }
			}}
		
#line 651 "hpricot_css.c"

		goto _st87;
		_ctr181:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 661 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{te = p;p = p - 1;{
#line 85 "hpricot_css.rl"
					FILTER(PSUEDO); }
			}}
		
#line 669 "hpricot_css.c"

		goto _st87;
		_ctr188:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 679 "hpricot_css.c"

			{
#line 84 "hpricot_css.rl"
			{te = p;p = p - 1;{
#line 84 "hpricot_css.rl"
					FILTER(POS); }
			}}
		
#line 687 "hpricot_css.c"

		goto _st87;
		_ctr207:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 697 "hpricot_css.c"

			{
#line 83 "hpricot_css.rl"
			{te = p;p = p - 1;{
#line 83 "hpricot_css.rl"
					FILTER(CHILD); }
			}}
		
#line 705 "hpricot_css.c"

		goto _st87;
		_ctr215:
			{
#line 80 "hpricot_css.rl"
			{te = p;p = p - 1;{
#line 80 "hpricot_css.rl"
					FILTER(ATTR); }
			}}
		
#line 715 "hpricot_css.c"

		goto _st87;
		_st87:
		if ( p == eof )
			goto _out87;
			{
#line 1 "NONE"
			{ts = 0;}}
		
#line 724 "hpricot_css.c"

			{
#line 1 "NONE"
			{act = 0;}}
		
#line 729 "hpricot_css.c"

		p+= 1;
		st_case_87:
		if ( p == pe && p != eof )
			goto _out87;
			{
#line 1 "NONE"
			{ts = p;}}
		
#line 738 "hpricot_css.c"

		if ( p == eof ) {
			goto _st87;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr152;
				}
				case 32: {
					goto _ctr156;
				}
				case 35: {
					goto _st7;
				}
				case 43: {
					goto _st92;
				}
				case 44: {
					goto _st90;
				}
				case 45: {
					goto _ctr158;
				}
				case 46: {
					goto _st13;
				}
				case 58: {
					goto _st19;
				}
				case 62: {
					goto _ctr159;
				}
				case 91: {
					goto _st52;
				}
				case 92: {
					goto _ctr161;
				}
				case 95: {
					goto _ctr160;
				}
				case 101: {
					goto _ctr162;
				}
				case 110: {
					goto _ctr158;
				}
				case 111: {
					goto _ctr163;
				}
				case 126: {
					goto _ctr159;
				}
			}
			if ( ( (*( p))) < 9 ) {
				if ( ( (*( p))) < -32 ) {
					if ( -59 <= ( (*( p))) ) {
						goto _ctr153;
					}
				} else if ( ( (*( p))) > -17 ) {
					if ( ( (*( p))) <= -12 ) {
						goto _ctr155;
					}
				} else {
					goto _ctr154;
				}
			} else if ( ( (*( p))) > 13 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr158;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr160;
					}
				} else {
					goto _ctr160;
				}
			} else {
				goto _ctr156;
			}
			goto _st0;
		}
		_st0:
		if ( p == eof )
			goto _out0;
		st_case_0:
		goto _out0;
		_ctr152:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 833 "hpricot_css.c"

		goto _st1;
		_st1:
		if ( p == eof )
			goto _out1;
		p+= 1;
		st_case_1:
		if ( p == pe && p != eof )
			goto _out1;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( -88 <= ( (*( p))) && ( (*( p))) <= -65 ) {
				goto _ctr2;
			}
			goto _ctr1;
		}
		_ctr2:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 855 "hpricot_css.c"

			{
#line 81 "hpricot_css.rl"
			{act = 5;}}
		
#line 860 "hpricot_css.c"

		goto _st88;
		_ctr160:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 867 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 874 "hpricot_css.c"

			{
#line 81 "hpricot_css.rl"
			{act = 5;}}
		
#line 879 "hpricot_css.c"

		goto _st88;
		_st88:
		if ( p == eof )
			goto _out88;
		p+= 1;
		st_case_88:
		if ( p == pe && p != eof )
			goto _out88;
		if ( p == eof ) {
			goto _ctr164;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st1;
				}
				case 45: {
					goto _ctr2;
				}
				case 92: {
					goto _st5;
				}
				case 95: {
					goto _ctr2;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st3;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st2;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr2;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr2;
					}
				} else {
					goto _ctr2;
				}
			} else {
				goto _st4;
			}
			goto _ctr164;
		}
		_ctr153:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 937 "hpricot_css.c"

		goto _st2;
		_st2:
		if ( p == eof )
			goto _out2;
		p+= 1;
		st_case_2:
		if ( p == pe && p != eof )
			goto _out2;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _ctr2;
			}
			goto _ctr1;
		}
		_ctr154:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 961 "hpricot_css.c"

		goto _st3;
		_st3:
		if ( p == eof )
			goto _out3;
		p+= 1;
		st_case_3:
		if ( p == pe && p != eof )
			goto _out3;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st2;
			}
			goto _ctr1;
		}
		_ctr155:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 985 "hpricot_css.c"

		goto _st4;
		_st4:
		if ( p == eof )
			goto _out4;
		p+= 1;
		st_case_4:
		if ( p == pe && p != eof )
			goto _out4;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st3;
			}
			goto _ctr1;
		}
		_ctr161:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1009 "hpricot_css.c"

		goto _st5;
		_st5:
		if ( p == eof )
			goto _out5;
		p+= 1;
		st_case_5:
		if ( p == pe && p != eof )
			goto _out5;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 46 ) {
				goto _ctr2;
			}
			goto _ctr1;
		}
		_ctr156:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 1031 "hpricot_css.c"

		goto _st89;
		_st89:
		if ( p == eof )
			goto _out89;
		p+= 1;
		st_case_89:
		if ( p == pe && p != eof )
			goto _out89;
		if ( p == eof ) {
			goto _ctr168;}
		else {
			switch( ( (*( p))) ) {
				case 32: {
					goto _st6;
				}
				case 44: {
					goto _st90;
				}
			}
			if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
				goto _st6;
			}
			goto _ctr168;
		}
		_st6:
		if ( p == eof )
			goto _out6;
		p+= 1;
		st_case_6:
		if ( p == pe && p != eof )
			goto _out6;
		if ( p == eof ) {
			goto _ctr5;}
		else {
			switch( ( (*( p))) ) {
				case 32: {
					goto _st6;
				}
				case 44: {
					goto _st90;
				}
			}
			if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
				goto _st6;
			}
			goto _ctr5;
		}
		_st90:
		if ( p == eof )
			goto _out90;
		p+= 1;
		st_case_90:
		if ( p == pe && p != eof )
			goto _out90;
		if ( p == eof ) {
			goto _ctr169;}
		else {
			if ( ( (*( p))) == 32 ) {
				goto _st90;
			}
			if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
				goto _st90;
			}
			goto _ctr169;
		}
		_st7:
		if ( p == eof )
			goto _out7;
		p+= 1;
		st_case_7:
		if ( p == pe && p != eof )
			goto _out7;
		if ( p == eof ) {
			goto _st7;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr9;
				}
				case 45: {
					goto _ctr13;
				}
				case 92: {
					goto _ctr14;
				}
				case 95: {
					goto _ctr13;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _ctr11;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _ctr10;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr13;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr13;
					}
				} else {
					goto _ctr13;
				}
			} else {
				goto _ctr12;
			}
			goto _st0;
		}
		_ctr9:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1153 "hpricot_css.c"

		goto _st8;
		_st8:
		if ( p == eof )
			goto _out8;
		p+= 1;
		st_case_8:
		if ( p == pe && p != eof )
			goto _out8;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( -88 <= ( (*( p))) && ( (*( p))) <= -65 ) {
				goto _ctr15;
			}
			goto _ctr1;
		}
		_ctr13:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 1175 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1182 "hpricot_css.c"

			{
#line 77 "hpricot_css.rl"
			{act = 1;}}
		
#line 1187 "hpricot_css.c"

		goto _st91;
		_ctr15:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 1194 "hpricot_css.c"

			{
#line 77 "hpricot_css.rl"
			{act = 1;}}
		
#line 1199 "hpricot_css.c"

		goto _st91;
		_st91:
		if ( p == eof )
			goto _out91;
		p+= 1;
		st_case_91:
		if ( p == pe && p != eof )
			goto _out91;
		if ( p == eof ) {
			goto _ctr170;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st8;
				}
				case 45: {
					goto _ctr15;
				}
				case 92: {
					goto _st12;
				}
				case 95: {
					goto _ctr15;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st10;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st9;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr15;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr15;
					}
				} else {
					goto _ctr15;
				}
			} else {
				goto _st11;
			}
			goto _ctr170;
		}
		_ctr10:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1257 "hpricot_css.c"

		goto _st9;
		_st9:
		if ( p == eof )
			goto _out9;
		p+= 1;
		st_case_9:
		if ( p == pe && p != eof )
			goto _out9;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _ctr15;
			}
			goto _ctr1;
		}
		_ctr11:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1281 "hpricot_css.c"

		goto _st10;
		_st10:
		if ( p == eof )
			goto _out10;
		p+= 1;
		st_case_10:
		if ( p == pe && p != eof )
			goto _out10;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st9;
			}
			goto _ctr1;
		}
		_ctr12:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1305 "hpricot_css.c"

		goto _st11;
		_st11:
		if ( p == eof )
			goto _out11;
		p+= 1;
		st_case_11:
		if ( p == pe && p != eof )
			goto _out11;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st10;
			}
			goto _ctr1;
		}
		_ctr14:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1329 "hpricot_css.c"

		goto _st12;
		_st12:
		if ( p == eof )
			goto _out12;
		p+= 1;
		st_case_12:
		if ( p == pe && p != eof )
			goto _out12;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 46 ) {
				goto _ctr15;
			}
			goto _ctr1;
		}
		_ctr175:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 1354 "hpricot_css.c"

		goto _st92;
		_st92:
		if ( p == eof )
			goto _out92;
		p+= 1;
		st_case_92:
		if ( p == pe && p != eof )
			goto _out92;
		if ( p == eof ) {
			goto _ctr174;}
		else {
			switch( ( (*( p))) ) {
				case 43: {
					goto _st92;
				}
				case 45: {
					goto _st92;
				}
				case 110: {
					goto _st92;
				}
			}
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _st92;
			}
			goto _ctr174;
		}
		_ctr176:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 1387 "hpricot_css.c"

			{
#line 81 "hpricot_css.rl"
			{act = 5;}}
		
#line 1392 "hpricot_css.c"

		goto _st93;
		_ctr158:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 1399 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1406 "hpricot_css.c"

			{
#line 81 "hpricot_css.rl"
			{act = 5;}}
		
#line 1411 "hpricot_css.c"

		goto _st93;
		_st93:
		if ( p == eof )
			goto _out93;
		p+= 1;
		st_case_93:
		if ( p == pe && p != eof )
			goto _out93;
		if ( p == eof ) {
			goto _ctr164;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st1;
				}
				case 43: {
					goto _ctr175;
				}
				case 45: {
					goto _ctr176;
				}
				case 92: {
					goto _st5;
				}
				case 95: {
					goto _ctr2;
				}
				case 110: {
					goto _ctr176;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st3;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st2;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr176;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr2;
					}
				} else {
					goto _ctr2;
				}
			} else {
				goto _st4;
			}
			goto _ctr164;
		}
		_st13:
		if ( p == eof )
			goto _out13;
		p+= 1;
		st_case_13:
		if ( p == pe && p != eof )
			goto _out13;
		if ( p == eof ) {
			goto _st13;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr19;
				}
				case 45: {
					goto _ctr23;
				}
				case 92: {
					goto _ctr24;
				}
				case 95: {
					goto _ctr23;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _ctr21;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _ctr20;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else {
					goto _ctr23;
				}
			} else {
				goto _ctr22;
			}
			goto _st0;
		}
		_ctr19:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1524 "hpricot_css.c"

		goto _st14;
		_st14:
		if ( p == eof )
			goto _out14;
		p+= 1;
		st_case_14:
		if ( p == pe && p != eof )
			goto _out14;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( -88 <= ( (*( p))) && ( (*( p))) <= -65 ) {
				goto _ctr25;
			}
			goto _ctr1;
		}
		_ctr23:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 1546 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1553 "hpricot_css.c"

			{
#line 78 "hpricot_css.rl"
			{act = 2;}}
		
#line 1558 "hpricot_css.c"

		goto _st94;
		_ctr25:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 1565 "hpricot_css.c"

			{
#line 78 "hpricot_css.rl"
			{act = 2;}}
		
#line 1570 "hpricot_css.c"

		goto _st94;
		_st94:
		if ( p == eof )
			goto _out94;
		p+= 1;
		st_case_94:
		if ( p == pe && p != eof )
			goto _out94;
		if ( p == eof ) {
			goto _ctr177;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st14;
				}
				case 45: {
					goto _ctr25;
				}
				case 92: {
					goto _st18;
				}
				case 95: {
					goto _ctr25;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st16;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st15;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr25;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr25;
					}
				} else {
					goto _ctr25;
				}
			} else {
				goto _st17;
			}
			goto _ctr177;
		}
		_ctr20:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1628 "hpricot_css.c"

		goto _st15;
		_st15:
		if ( p == eof )
			goto _out15;
		p+= 1;
		st_case_15:
		if ( p == pe && p != eof )
			goto _out15;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _ctr25;
			}
			goto _ctr1;
		}
		_ctr21:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1652 "hpricot_css.c"

		goto _st16;
		_st16:
		if ( p == eof )
			goto _out16;
		p+= 1;
		st_case_16:
		if ( p == pe && p != eof )
			goto _out16;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st15;
			}
			goto _ctr1;
		}
		_ctr22:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1676 "hpricot_css.c"

		goto _st17;
		_st17:
		if ( p == eof )
			goto _out17;
		p+= 1;
		st_case_17:
		if ( p == pe && p != eof )
			goto _out17;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st16;
			}
			goto _ctr1;
		}
		_ctr24:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1700 "hpricot_css.c"

		goto _st18;
		_st18:
		if ( p == eof )
			goto _out18;
		p+= 1;
		st_case_18:
		if ( p == pe && p != eof )
			goto _out18;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 46 ) {
				goto _ctr25;
			}
			goto _ctr1;
		}
		_st19:
		if ( p == eof )
			goto _out19;
		p+= 1;
		st_case_19:
		if ( p == pe && p != eof )
			goto _out19;
		if ( p == eof ) {
			goto _st19;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr29;
				}
				case 45: {
					goto _ctr33;
				}
				case 92: {
					goto _ctr34;
				}
				case 95: {
					goto _ctr33;
				}
				case 101: {
					goto _ctr35;
				}
				case 102: {
					goto _ctr36;
				}
				case 103: {
					goto _ctr37;
				}
				case 108: {
					goto _ctr38;
				}
				case 110: {
					goto _ctr39;
				}
				case 111: {
					goto _ctr40;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _ctr31;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _ctr30;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr33;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr33;
					}
				} else {
					goto _ctr33;
				}
			} else {
				goto _ctr32;
			}
			goto _st0;
		}
		_ctr29:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1791 "hpricot_css.c"

		goto _st20;
		_ctr189:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 1801 "hpricot_css.c"

		goto _st20;
		_st20:
		if ( p == eof )
			goto _out20;
		p+= 1;
		st_case_20:
		if ( p == pe && p != eof )
			goto _out20;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( -88 <= ( (*( p))) && ( (*( p))) <= -65 ) {
				goto _ctr41;
			}
			goto _ctr1;
		}
		_ctr33:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 1823 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1830 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 1835 "hpricot_css.c"

		goto _st95;
		_ctr41:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 1842 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 1847 "hpricot_css.c"

		goto _st95;
		_ctr194:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 1854 "hpricot_css.c"

			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 1862 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 1867 "hpricot_css.c"

		goto _st95;
		_st95:
		if ( p == eof )
			goto _out95;
		p+= 1;
		st_case_95:
		if ( p == pe && p != eof )
			goto _out95;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr30:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1928 "hpricot_css.c"

		goto _st21;
		_ctr190:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 1938 "hpricot_css.c"

		goto _st21;
		_st21:
		if ( p == eof )
			goto _out21;
		p+= 1;
		st_case_21:
		if ( p == pe && p != eof )
			goto _out21;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _ctr41;
			}
			goto _ctr1;
		}
		_ctr31:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1962 "hpricot_css.c"

		goto _st22;
		_ctr191:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 1972 "hpricot_css.c"

		goto _st22;
		_st22:
		if ( p == eof )
			goto _out22;
		p+= 1;
		st_case_22:
		if ( p == pe && p != eof )
			goto _out22;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st21;
			}
			goto _ctr1;
		}
		_ctr32:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 1996 "hpricot_css.c"

		goto _st23;
		_ctr192:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 2006 "hpricot_css.c"

		goto _st23;
		_st23:
		if ( p == eof )
			goto _out23;
		p+= 1;
		st_case_23:
		if ( p == pe && p != eof )
			goto _out23;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st22;
			}
			goto _ctr1;
		}
		_ctr184:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 2031 "hpricot_css.c"

		goto _st24;
		_st24:
		if ( p == eof )
			goto _out24;
		p+= 1;
		st_case_24:
		if ( p == pe && p != eof )
			goto _out24;
		if ( p == eof ) {
			goto _ctr44;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr46;
				}
				case 39: {
					goto _ctr47;
				}
				case 40: {
					goto _ctr48;
				}
				case 41: {
					goto _ctr49;
				}
			}
			goto _ctr45;
		}
		_ctr45:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 2066 "hpricot_css.c"

		goto _st25;
		_st25:
		if ( p == eof )
			goto _out25;
		p+= 1;
		st_case_25:
		if ( p == pe && p != eof )
			goto _out25;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr1;
				}
				case 40: {
					goto _ctr1;
				}
				case 41: {
					goto _ctr51;
				}
			}
			goto _st25;
		}
		_ctr46:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 2098 "hpricot_css.c"

		goto _st26;
		_st26:
		if ( p == eof )
			goto _out26;
		p+= 1;
		st_case_26:
		if ( p == pe && p != eof )
			goto _out26;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _st28;
				}
				case 40: {
					goto _st29;
				}
				case 41: {
					goto _ctr1;
				}
			}
			goto _st27;
		}
		_st27:
		if ( p == eof )
			goto _out27;
		p+= 1;
		st_case_27:
		if ( p == pe && p != eof )
			goto _out27;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 34 ) {
				goto _st28;
			}
			if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
				goto _ctr1;
			}
			goto _st27;
		}
		_st28:
		if ( p == eof )
			goto _out28;
		p+= 1;
		st_case_28:
		if ( p == pe && p != eof )
			goto _out28;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 41 ) {
				goto _ctr51;
			}
			goto _ctr1;
		}
		_st29:
		if ( p == eof )
			goto _out29;
		p+= 1;
		st_case_29:
		if ( p == pe && p != eof )
			goto _out29;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 41 ) {
				goto _ctr1;
			}
			goto _st30;
		}
		_st30:
		if ( p == eof )
			goto _out30;
		p+= 1;
		st_case_30:
		if ( p == pe && p != eof )
			goto _out30;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 41 ) {
				goto _st31;
			}
			goto _st30;
		}
		_st31:
		if ( p == eof )
			goto _out31;
		p+= 1;
		st_case_31:
		if ( p == pe && p != eof )
			goto _out31;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _st28;
				}
				case 40: {
					goto _st29;
				}
			}
			goto _ctr1;
		}
		_ctr47:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 2213 "hpricot_css.c"

		goto _st32;
		_st32:
		if ( p == eof )
			goto _out32;
		p+= 1;
		st_case_32:
		if ( p == pe && p != eof )
			goto _out32;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _st34;
				}
				case 39: {
					goto _st25;
				}
				case 40: {
					goto _st35;
				}
				case 41: {
					goto _ctr51;
				}
			}
			goto _st33;
		}
		_st33:
		if ( p == eof )
			goto _out33;
		p+= 1;
		st_case_33:
		if ( p == pe && p != eof )
			goto _out33;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _st34;
				}
				case 39: {
					goto _st25;
				}
				case 40: {
					goto _ctr1;
				}
				case 41: {
					goto _ctr51;
				}
			}
			goto _st33;
		}
		_st34:
		if ( p == eof )
			goto _out34;
		p+= 1;
		st_case_34:
		if ( p == pe && p != eof )
			goto _out34;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 39 ) {
				goto _st28;
			}
			if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
				goto _ctr1;
			}
			goto _st34;
		}
		_st35:
		if ( p == eof )
			goto _out35;
		p+= 1;
		st_case_35:
		if ( p == pe && p != eof )
			goto _out35;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 41 ) {
				goto _ctr1;
			}
			goto _st36;
		}
		_st36:
		if ( p == eof )
			goto _out36;
		p+= 1;
		st_case_36:
		if ( p == pe && p != eof )
			goto _out36;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 41 ) {
				goto _st37;
			}
			goto _st36;
		}
		_st37:
		if ( p == eof )
			goto _out37;
		p+= 1;
		st_case_37:
		if ( p == pe && p != eof )
			goto _out37;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			switch( ( (*( p))) ) {
				case 39: {
					goto _st28;
				}
				case 40: {
					goto _st35;
				}
			}
			goto _ctr1;
		}
		_ctr48:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 2342 "hpricot_css.c"

		goto _st38;
		_st38:
		if ( p == eof )
			goto _out38;
		p+= 1;
		st_case_38:
		if ( p == pe && p != eof )
			goto _out38;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 41 ) {
				goto _ctr1;
			}
			goto _st39;
		}
		_st39:
		if ( p == eof )
			goto _out39;
		p+= 1;
		st_case_39:
		if ( p == pe && p != eof )
			goto _out39;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 41 ) {
				goto _st40;
			}
			goto _st39;
		}
		_st40:
		if ( p == eof )
			goto _out40;
		p+= 1;
		st_case_40:
		if ( p == pe && p != eof )
			goto _out40;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			switch( ( (*( p))) ) {
				case 40: {
					goto _st38;
				}
				case 41: {
					goto _ctr51;
				}
			}
			goto _ctr1;
		}
		_ctr34:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 2401 "hpricot_css.c"

		goto _st41;
		_ctr195:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 2411 "hpricot_css.c"

		goto _st41;
		_st41:
		if ( p == eof )
			goto _out41;
		p+= 1;
		st_case_41:
		if ( p == pe && p != eof )
			goto _out41;
		if ( p == eof ) {
			goto _ctr1;}
		else {
			if ( ( (*( p))) == 46 ) {
				goto _ctr41;
			}
			goto _ctr1;
		}
		_ctr35:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 2433 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 2440 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 2445 "hpricot_css.c"

		goto _st96;
		_st96:
		if ( p == eof )
			goto _out96;
		p+= 1;
		st_case_96:
		if ( p == pe && p != eof )
			goto _out96;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 113: {
					goto _ctr186;
				}
				case 118: {
					goto _ctr187;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr186:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 2510 "hpricot_css.c"

			{
#line 84 "hpricot_css.rl"
			{act = 8;}}
		
#line 2515 "hpricot_css.c"

		goto _st97;
		_st97:
		if ( p == eof )
			goto _out97;
		p+= 1;
		st_case_97:
		if ( p == pe && p != eof )
			goto _out97;
		if ( p == eof ) {
			goto _ctr188;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr189;
				}
				case 40: {
					goto _ctr193;
				}
				case 45: {
					goto _ctr194;
				}
				case 92: {
					goto _ctr195;
				}
				case 95: {
					goto _ctr194;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _ctr191;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _ctr190;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr194;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr194;
					}
				} else {
					goto _ctr194;
				}
			} else {
				goto _ctr192;
			}
			goto _ctr188;
		}
		_ctr193:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 2577 "hpricot_css.c"

		goto _st42;
		_st42:
		if ( p == eof )
			goto _out42;
		p+= 1;
		st_case_42:
		if ( p == pe && p != eof )
			goto _out42;
		if ( p == eof ) {
			goto _ctr65;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr46;
				}
				case 39: {
					goto _ctr47;
				}
				case 40: {
					goto _ctr48;
				}
				case 41: {
					goto _ctr49;
				}
			}
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _ctr66;
			}
			goto _ctr45;
		}
		_ctr66:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 2615 "hpricot_css.c"

		goto _st43;
		_st43:
		if ( p == eof )
			goto _out43;
		p+= 1;
		st_case_43:
		if ( p == pe && p != eof )
			goto _out43;
		if ( p == eof ) {
			goto _ctr65;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr65;
				}
				case 40: {
					goto _ctr65;
				}
				case 41: {
					goto _ctr67;
				}
			}
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _st43;
			}
			goto _st25;
		}
		_ctr187:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 2648 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 2653 "hpricot_css.c"

		goto _st98;
		_st98:
		if ( p == eof )
			goto _out98;
		p+= 1;
		st_case_98:
		if ( p == pe && p != eof )
			goto _out98;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 101: {
					goto _ctr196;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr196:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 2715 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 2720 "hpricot_css.c"

		goto _st99;
		_st99:
		if ( p == eof )
			goto _out99;
		p+= 1;
		st_case_99:
		if ( p == pe && p != eof )
			goto _out99;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 110: {
					goto _ctr186;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr36:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 2782 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 2789 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 2794 "hpricot_css.c"

		goto _st100;
		_st100:
		if ( p == eof )
			goto _out100;
		p+= 1;
		st_case_100:
		if ( p == pe && p != eof )
			goto _out100;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 105: {
					goto _ctr197;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr197:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 2856 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 2861 "hpricot_css.c"

		goto _st101;
		_st101:
		if ( p == eof )
			goto _out101;
		p+= 1;
		st_case_101:
		if ( p == pe && p != eof )
			goto _out101;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 114: {
					goto _ctr198;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr198:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 2923 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 2928 "hpricot_css.c"

		goto _st102;
		_st102:
		if ( p == eof )
			goto _out102;
		p+= 1;
		st_case_102:
		if ( p == pe && p != eof )
			goto _out102;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 115: {
					goto _ctr199;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr199:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 2990 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 2995 "hpricot_css.c"

		goto _st103;
		_st103:
		if ( p == eof )
			goto _out103;
		p+= 1;
		st_case_103:
		if ( p == pe && p != eof )
			goto _out103;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 116: {
					goto _ctr200;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr200:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3057 "hpricot_css.c"

			{
#line 84 "hpricot_css.rl"
			{act = 8;}}
		
#line 3062 "hpricot_css.c"

		goto _st104;
		_st104:
		if ( p == eof )
			goto _out104;
		p+= 1;
		st_case_104:
		if ( p == pe && p != eof )
			goto _out104;
		if ( p == eof ) {
			goto _ctr188;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr189;
				}
				case 40: {
					goto _ctr193;
				}
				case 45: {
					goto _ctr201;
				}
				case 92: {
					goto _ctr195;
				}
				case 95: {
					goto _ctr194;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _ctr191;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _ctr190;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr194;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr194;
					}
				} else {
					goto _ctr194;
				}
			} else {
				goto _ctr192;
			}
			goto _ctr188;
		}
		_ctr214:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3121 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 3126 "hpricot_css.c"

		goto _st105;
		_ctr201:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3133 "hpricot_css.c"

			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 3141 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 3146 "hpricot_css.c"

		goto _st105;
		_st105:
		if ( p == eof )
			goto _out105;
		p+= 1;
		st_case_105:
		if ( p == pe && p != eof )
			goto _out105;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 99: {
					goto _ctr202;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr202:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3208 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 3213 "hpricot_css.c"

		goto _st106;
		_st106:
		if ( p == eof )
			goto _out106;
		p+= 1;
		st_case_106:
		if ( p == pe && p != eof )
			goto _out106;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 104: {
					goto _ctr203;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr203:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3275 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 3280 "hpricot_css.c"

		goto _st107;
		_st107:
		if ( p == eof )
			goto _out107;
		p+= 1;
		st_case_107:
		if ( p == pe && p != eof )
			goto _out107;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 105: {
					goto _ctr204;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr204:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3342 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 3347 "hpricot_css.c"

		goto _st108;
		_st108:
		if ( p == eof )
			goto _out108;
		p+= 1;
		st_case_108:
		if ( p == pe && p != eof )
			goto _out108;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 108: {
					goto _ctr205;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr205:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3409 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 3414 "hpricot_css.c"

		goto _st109;
		_st109:
		if ( p == eof )
			goto _out109;
		p+= 1;
		st_case_109:
		if ( p == pe && p != eof )
			goto _out109;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 100: {
					goto _ctr206;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr206:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3476 "hpricot_css.c"

			{
#line 83 "hpricot_css.rl"
			{act = 7;}}
		
#line 3481 "hpricot_css.c"

		goto _st110;
		_st110:
		if ( p == eof )
			goto _out110;
		p+= 1;
		st_case_110:
		if ( p == pe && p != eof )
			goto _out110;
		if ( p == eof ) {
			goto _ctr207;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr189;
				}
				case 40: {
					goto _ctr208;
				}
				case 45: {
					goto _ctr194;
				}
				case 92: {
					goto _ctr195;
				}
				case 95: {
					goto _ctr194;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _ctr191;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _ctr190;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr194;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr194;
					}
				} else {
					goto _ctr194;
				}
			} else {
				goto _ctr192;
			}
			goto _ctr207;
		}
		_ctr208:
			{
#line 34 "hpricot_css.rl"
			
			ape = p;
			PUSH(aps, ape);
		}
		
#line 3543 "hpricot_css.c"

		goto _st44;
		_st44:
		if ( p == eof )
			goto _out44;
		p+= 1;
		st_case_44:
		if ( p == pe && p != eof )
			goto _out44;
		if ( p == eof ) {
			goto _ctr69;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr46;
				}
				case 39: {
					goto _ctr47;
				}
				case 40: {
					goto _ctr48;
				}
				case 41: {
					goto _ctr70;
				}
				case 43: {
					goto _ctr71;
				}
				case 45: {
					goto _ctr71;
				}
				case 101: {
					goto _ctr72;
				}
				case 110: {
					goto _ctr71;
				}
				case 111: {
					goto _ctr73;
				}
			}
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _ctr71;
			}
			goto _ctr45;
		}
		_ctr71:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 3596 "hpricot_css.c"

		goto _st45;
		_st45:
		if ( p == eof )
			goto _out45;
		p+= 1;
		st_case_45:
		if ( p == pe && p != eof )
			goto _out45;
		if ( p == eof ) {
			goto _ctr69;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr69;
				}
				case 40: {
					goto _ctr69;
				}
				case 41: {
					goto _ctr74;
				}
				case 43: {
					goto _st45;
				}
				case 45: {
					goto _st45;
				}
				case 110: {
					goto _st45;
				}
			}
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _st45;
			}
			goto _st25;
		}
		_ctr72:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 3640 "hpricot_css.c"

		goto _st46;
		_st46:
		if ( p == eof )
			goto _out46;
		p+= 1;
		st_case_46:
		if ( p == pe && p != eof )
			goto _out46;
		if ( p == eof ) {
			goto _ctr69;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr69;
				}
				case 40: {
					goto _ctr69;
				}
				case 41: {
					goto _ctr51;
				}
				case 118: {
					goto _st47;
				}
			}
			goto _st25;
		}
		_st47:
		if ( p == eof )
			goto _out47;
		p+= 1;
		st_case_47:
		if ( p == pe && p != eof )
			goto _out47;
		if ( p == eof ) {
			goto _ctr69;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr69;
				}
				case 40: {
					goto _ctr69;
				}
				case 41: {
					goto _ctr51;
				}
				case 101: {
					goto _st48;
				}
			}
			goto _st25;
		}
		_st48:
		if ( p == eof )
			goto _out48;
		p+= 1;
		st_case_48:
		if ( p == pe && p != eof )
			goto _out48;
		if ( p == eof ) {
			goto _ctr69;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr69;
				}
				case 40: {
					goto _ctr69;
				}
				case 41: {
					goto _ctr51;
				}
				case 110: {
					goto _st49;
				}
			}
			goto _st25;
		}
		_st49:
		if ( p == eof )
			goto _out49;
		p+= 1;
		st_case_49:
		if ( p == pe && p != eof )
			goto _out49;
		if ( p == eof ) {
			goto _ctr69;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr69;
				}
				case 40: {
					goto _ctr69;
				}
				case 41: {
					goto _ctr74;
				}
			}
			goto _st25;
		}
		_ctr73:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 3750 "hpricot_css.c"

		goto _st50;
		_st50:
		if ( p == eof )
			goto _out50;
		p+= 1;
		st_case_50:
		if ( p == pe && p != eof )
			goto _out50;
		if ( p == eof ) {
			goto _ctr69;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr69;
				}
				case 40: {
					goto _ctr69;
				}
				case 41: {
					goto _ctr51;
				}
				case 100: {
					goto _st51;
				}
			}
			goto _st25;
		}
		_st51:
		if ( p == eof )
			goto _out51;
		p+= 1;
		st_case_51:
		if ( p == pe && p != eof )
			goto _out51;
		if ( p == eof ) {
			goto _ctr69;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _ctr69;
				}
				case 40: {
					goto _ctr69;
				}
				case 41: {
					goto _ctr51;
				}
				case 100: {
					goto _st49;
				}
			}
			goto _st25;
		}
		_ctr37:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3809 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 3816 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 3821 "hpricot_css.c"

		goto _st111;
		_st111:
		if ( p == eof )
			goto _out111;
		p+= 1;
		st_case_111:
		if ( p == pe && p != eof )
			goto _out111;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 116: {
					goto _ctr186;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr38:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3883 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 3890 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 3895 "hpricot_css.c"

		goto _st112;
		_st112:
		if ( p == eof )
			goto _out112;
		p+= 1;
		st_case_112:
		if ( p == pe && p != eof )
			goto _out112;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 97: {
					goto _ctr198;
				}
				case 116: {
					goto _ctr186;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr39:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 3960 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 3967 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 3972 "hpricot_css.c"

		goto _st113;
		_st113:
		if ( p == eof )
			goto _out113;
		p+= 1;
		st_case_113:
		if ( p == pe && p != eof )
			goto _out113;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 116: {
					goto _ctr209;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr209:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 4034 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 4039 "hpricot_css.c"

		goto _st114;
		_st114:
		if ( p == eof )
			goto _out114;
		p+= 1;
		st_case_114:
		if ( p == pe && p != eof )
			goto _out114;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 104: {
					goto _ctr200;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr40:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 4101 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 4108 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 4113 "hpricot_css.c"

		goto _st115;
		_st115:
		if ( p == eof )
			goto _out115;
		p+= 1;
		st_case_115:
		if ( p == pe && p != eof )
			goto _out115;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 100: {
					goto _ctr210;
				}
				case 110: {
					goto _ctr211;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr210:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 4178 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 4183 "hpricot_css.c"

		goto _st116;
		_st116:
		if ( p == eof )
			goto _out116;
		p+= 1;
		st_case_116:
		if ( p == pe && p != eof )
			goto _out116;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 100: {
					goto _ctr186;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr211:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 4245 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 4250 "hpricot_css.c"

		goto _st117;
		_st117:
		if ( p == eof )
			goto _out117;
		p+= 1;
		st_case_117:
		if ( p == pe && p != eof )
			goto _out117;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 108: {
					goto _ctr212;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr212:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 4312 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 4317 "hpricot_css.c"

		goto _st118;
		_st118:
		if ( p == eof )
			goto _out118;
		p+= 1;
		st_case_118:
		if ( p == pe && p != eof )
			goto _out118;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr41;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
				case 121: {
					goto _ctr213;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_ctr213:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 4379 "hpricot_css.c"

			{
#line 85 "hpricot_css.rl"
			{act = 9;}}
		
#line 4384 "hpricot_css.c"

		goto _st119;
		_st119:
		if ( p == eof )
			goto _out119;
		p+= 1;
		st_case_119:
		if ( p == pe && p != eof )
			goto _out119;
		if ( p == eof ) {
			goto _ctr181;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st20;
				}
				case 40: {
					goto _ctr184;
				}
				case 45: {
					goto _ctr214;
				}
				case 92: {
					goto _st41;
				}
				case 95: {
					goto _ctr41;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st22;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st21;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
			} else {
				goto _st23;
			}
			goto _ctr181;
		}
		_st52:
		if ( p == eof )
			goto _out52;
		p+= 1;
		st_case_52:
		if ( p == pe && p != eof )
			goto _out52;
		if ( p == eof ) {
			goto _st52;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr81;
				}
				case 45: {
					goto _ctr85;
				}
				case 92: {
					goto _ctr86;
				}
				case 95: {
					goto _ctr85;
				}
				case 110: {
					goto _ctr87;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _ctr83;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _ctr82;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr85;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr85;
					}
				} else {
					goto _ctr85;
				}
			} else {
				goto _ctr84;
			}
			goto _st0;
		}
		_ctr81:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 4497 "hpricot_css.c"

		goto _st53;
		_st53:
		if ( p == eof )
			goto _out53;
		p+= 1;
		st_case_53:
		if ( p == pe && p != eof )
			goto _out53;
		if ( p == eof ) {
			goto _st53;}
		else {
			if ( -88 <= ( (*( p))) && ( (*( p))) <= -65 ) {
				goto _st54;
			}
			goto _st0;
		}
		_ctr85:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 4521 "hpricot_css.c"

		goto _st54;
		_ctr96:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 4531 "hpricot_css.c"

		goto _st54;
		_st54:
		if ( p == eof )
			goto _out54;
		p+= 1;
		st_case_54:
		if ( p == pe && p != eof )
			goto _out54;
		if ( p == eof ) {
			goto _st54;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr91;
				}
				case 32: {
					goto _ctr95;
				}
				case 45: {
					goto _ctr96;
				}
				case 61: {
					goto _ctr97;
				}
				case 92: {
					goto _ctr98;
				}
				case 95: {
					goto _ctr96;
				}
			}
			if ( ( (*( p))) < 9 ) {
				if ( ( (*( p))) < -32 ) {
					if ( -59 <= ( (*( p))) ) {
						goto _ctr92;
					}
				} else if ( ( (*( p))) > -17 ) {
					if ( ( (*( p))) <= -12 ) {
						goto _ctr94;
					}
				} else {
					goto _ctr93;
				}
			} else if ( ( (*( p))) > 13 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr96;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr96;
					}
				} else {
					goto _ctr96;
				}
			} else {
				goto _ctr95;
			}
			goto _ctr90;
		}
		_ctr90:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 4600 "hpricot_css.c"

		goto _st55;
		_st55:
		if ( p == eof )
			goto _out55;
		p+= 1;
		st_case_55:
		if ( p == pe && p != eof )
			goto _out55;
		if ( p == eof ) {
			goto _st55;}
		else {
			if ( ( (*( p))) == 61 ) {
				goto _st56;
			}
			goto _st0;
		}
		_st56:
		if ( p == eof )
			goto _out56;
		p+= 1;
		st_case_56:
		if ( p == pe && p != eof )
			goto _out56;
		if ( p == eof ) {
			goto _st56;}
		else {
			switch( ( (*( p))) ) {
				case 32: {
					goto _ctr102;
				}
				case 34: {
					goto _ctr103;
				}
				case 39: {
					goto _ctr104;
				}
				case 93: {
					goto _st0;
				}
			}
			if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
				goto _ctr102;
			}
			goto _ctr101;
		}
		_ctr101:
			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 4655 "hpricot_css.c"

		goto _st57;
		_st57:
		if ( p == eof )
			goto _out57;
		p+= 1;
		st_case_57:
		if ( p == pe && p != eof )
			goto _out57;
		if ( p == eof ) {
			goto _st57;}
		else {
			if ( ( (*( p))) == 93 ) {
				goto _ctr106;
			}
			goto _st57;
		}
		_ctr102:
			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 4681 "hpricot_css.c"

		goto _st58;
		_st58:
		if ( p == eof )
			goto _out58;
		p+= 1;
		st_case_58:
		if ( p == pe && p != eof )
			goto _out58;
		if ( p == eof ) {
			goto _st58;}
		else {
			switch( ( (*( p))) ) {
				case 32: {
					goto _st58;
				}
				case 34: {
					goto _st59;
				}
				case 39: {
					goto _st62;
				}
				case 93: {
					goto _ctr106;
				}
			}
			if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
				goto _st58;
			}
			goto _st57;
		}
		_ctr103:
			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 4721 "hpricot_css.c"

		goto _st59;
		_st59:
		if ( p == eof )
			goto _out59;
		p+= 1;
		st_case_59:
		if ( p == pe && p != eof )
			goto _out59;
		if ( p == eof ) {
			goto _st59;}
		else {
			switch( ( (*( p))) ) {
				case 34: {
					goto _st57;
				}
				case 93: {
					goto _ctr110;
				}
			}
			goto _st59;
		}
		_ctr110:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 4748 "hpricot_css.c"

		goto _st120;
		_st120:
		if ( p == eof )
			goto _out120;
		p+= 1;
		st_case_120:
		if ( p == pe && p != eof )
			goto _out120;
		if ( p == eof ) {
			goto _ctr215;}
		else {
			if ( ( (*( p))) == 34 ) {
				goto _st61;
			}
			goto _st60;
		}
		_st60:
		if ( p == eof )
			goto _out60;
		p+= 1;
		st_case_60:
		if ( p == pe && p != eof )
			goto _out60;
		if ( p == eof ) {
			goto _ctr111;}
		else {
			if ( ( (*( p))) == 34 ) {
				goto _st61;
			}
			goto _st60;
		}
		_st61:
		if ( p == eof )
			goto _out61;
		p+= 1;
		st_case_61:
		if ( p == pe && p != eof )
			goto _out61;
		if ( p == eof ) {
			goto _ctr111;}
		else {
			if ( ( (*( p))) == 93 ) {
				goto _ctr106;
			}
			goto _ctr111;
		}
		_ctr104:
			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 4804 "hpricot_css.c"

		goto _st62;
		_st62:
		if ( p == eof )
			goto _out62;
		p+= 1;
		st_case_62:
		if ( p == pe && p != eof )
			goto _out62;
		if ( p == eof ) {
			goto _st62;}
		else {
			switch( ( (*( p))) ) {
				case 39: {
					goto _st57;
				}
				case 93: {
					goto _ctr114;
				}
			}
			goto _st62;
		}
		_ctr114:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 4831 "hpricot_css.c"

		goto _st121;
		_st121:
		if ( p == eof )
			goto _out121;
		p+= 1;
		st_case_121:
		if ( p == pe && p != eof )
			goto _out121;
		if ( p == eof ) {
			goto _ctr215;}
		else {
			if ( ( (*( p))) == 39 ) {
				goto _st61;
			}
			goto _st63;
		}
		_st63:
		if ( p == eof )
			goto _out63;
		p+= 1;
		st_case_63:
		if ( p == pe && p != eof )
			goto _out63;
		if ( p == eof ) {
			goto _ctr111;}
		else {
			if ( ( (*( p))) == 39 ) {
				goto _st61;
			}
			goto _st63;
		}
		_ctr91:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 4871 "hpricot_css.c"

		goto _st64;
		_st64:
		if ( p == eof )
			goto _out64;
		p+= 1;
		st_case_64:
		if ( p == pe && p != eof )
			goto _out64;
		if ( p == eof ) {
			goto _st64;}
		else {
			if ( ( (*( p))) == 61 ) {
				goto _st56;
			}
			if ( -88 <= ( (*( p))) && ( (*( p))) <= -65 ) {
				goto _st54;
			}
			goto _st0;
		}
		_ctr92:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 4899 "hpricot_css.c"

		goto _st65;
		_st65:
		if ( p == eof )
			goto _out65;
		p+= 1;
		st_case_65:
		if ( p == pe && p != eof )
			goto _out65;
		if ( p == eof ) {
			goto _st65;}
		else {
			if ( ( (*( p))) == 61 ) {
				goto _st56;
			}
			if ( ( (*( p))) <= -65 ) {
				goto _st54;
			}
			goto _st0;
		}
		_ctr93:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 4927 "hpricot_css.c"

		goto _st66;
		_st66:
		if ( p == eof )
			goto _out66;
		p+= 1;
		st_case_66:
		if ( p == pe && p != eof )
			goto _out66;
		if ( p == eof ) {
			goto _st66;}
		else {
			if ( ( (*( p))) == 61 ) {
				goto _st56;
			}
			if ( ( (*( p))) <= -65 ) {
				goto _st67;
			}
			goto _st0;
		}
		_ctr82:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 4954 "hpricot_css.c"

		goto _st67;
		_st67:
		if ( p == eof )
			goto _out67;
		p+= 1;
		st_case_67:
		if ( p == pe && p != eof )
			goto _out67;
		if ( p == eof ) {
			goto _st67;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st54;
			}
			goto _st0;
		}
		_ctr94:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 4979 "hpricot_css.c"

		goto _st68;
		_st68:
		if ( p == eof )
			goto _out68;
		p+= 1;
		st_case_68:
		if ( p == pe && p != eof )
			goto _out68;
		if ( p == eof ) {
			goto _st68;}
		else {
			if ( ( (*( p))) == 61 ) {
				goto _st56;
			}
			if ( ( (*( p))) <= -65 ) {
				goto _st69;
			}
			goto _st0;
		}
		_ctr83:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5006 "hpricot_css.c"

		goto _st69;
		_st69:
		if ( p == eof )
			goto _out69;
		p+= 1;
		st_case_69:
		if ( p == pe && p != eof )
			goto _out69;
		if ( p == eof ) {
			goto _st69;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st67;
			}
			goto _st0;
		}
		_ctr95:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 5031 "hpricot_css.c"

		goto _st70;
		_st70:
		if ( p == eof )
			goto _out70;
		p+= 1;
		st_case_70:
		if ( p == pe && p != eof )
			goto _out70;
		if ( p == eof ) {
			goto _st70;}
		else {
			switch( ( (*( p))) ) {
				case 32: {
					goto _st70;
				}
				case 61: {
					goto _st71;
				}
			}
			if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
				goto _st70;
			}
			goto _st55;
		}
		_ctr97:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 5064 "hpricot_css.c"

		goto _st71;
		_st71:
		if ( p == eof )
			goto _out71;
		p+= 1;
		st_case_71:
		if ( p == pe && p != eof )
			goto _out71;
		if ( p == eof ) {
			goto _st71;}
		else {
			switch( ( (*( p))) ) {
				case 32: {
					goto _ctr102;
				}
				case 34: {
					goto _ctr103;
				}
				case 39: {
					goto _ctr104;
				}
				case 61: {
					goto _ctr124;
				}
				case 93: {
					goto _st0;
				}
			}
			if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
				goto _ctr102;
			}
			goto _ctr101;
		}
		_ctr124:
			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 5107 "hpricot_css.c"

		goto _st72;
		_st72:
		if ( p == eof )
			goto _out72;
		p+= 1;
		st_case_72:
		if ( p == pe && p != eof )
			goto _out72;
		if ( p == eof ) {
			goto _st72;}
		else {
			switch( ( (*( p))) ) {
				case 32: {
					goto _ctr102;
				}
				case 34: {
					goto _ctr103;
				}
				case 39: {
					goto _ctr104;
				}
				case 93: {
					goto _ctr106;
				}
			}
			if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
				goto _ctr102;
			}
			goto _ctr101;
		}
		_ctr98:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 5146 "hpricot_css.c"

		goto _st73;
		_st73:
		if ( p == eof )
			goto _out73;
		p+= 1;
		st_case_73:
		if ( p == pe && p != eof )
			goto _out73;
		if ( p == eof ) {
			goto _st73;}
		else {
			switch( ( (*( p))) ) {
				case 46: {
					goto _st54;
				}
				case 61: {
					goto _st56;
				}
			}
			goto _st0;
		}
		_ctr84:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5175 "hpricot_css.c"

		goto _st74;
		_st74:
		if ( p == eof )
			goto _out74;
		p+= 1;
		st_case_74:
		if ( p == pe && p != eof )
			goto _out74;
		if ( p == eof ) {
			goto _st74;}
		else {
			if ( ( (*( p))) <= -65 ) {
				goto _st69;
			}
			goto _st0;
		}
		_ctr86:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5199 "hpricot_css.c"

		goto _st75;
		_st75:
		if ( p == eof )
			goto _out75;
		p+= 1;
		st_case_75:
		if ( p == pe && p != eof )
			goto _out75;
		if ( p == eof ) {
			goto _st75;}
		else {
			if ( ( (*( p))) == 46 ) {
				goto _st54;
			}
			goto _st0;
		}
		_ctr87:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5223 "hpricot_css.c"

		goto _st76;
		_st76:
		if ( p == eof )
			goto _out76;
		p+= 1;
		st_case_76:
		if ( p == pe && p != eof )
			goto _out76;
		if ( p == eof ) {
			goto _st76;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr91;
				}
				case 32: {
					goto _ctr95;
				}
				case 45: {
					goto _ctr96;
				}
				case 61: {
					goto _ctr97;
				}
				case 92: {
					goto _ctr98;
				}
				case 95: {
					goto _ctr96;
				}
				case 97: {
					goto _ctr130;
				}
			}
			if ( ( (*( p))) < 9 ) {
				if ( ( (*( p))) < -32 ) {
					if ( -59 <= ( (*( p))) ) {
						goto _ctr92;
					}
				} else if ( ( (*( p))) > -17 ) {
					if ( ( (*( p))) <= -12 ) {
						goto _ctr94;
					}
				} else {
					goto _ctr93;
				}
			} else if ( ( (*( p))) > 13 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr96;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr96;
					}
				} else {
					goto _ctr96;
				}
			} else {
				goto _ctr95;
			}
			goto _ctr90;
		}
		_ctr130:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 5295 "hpricot_css.c"

		goto _st77;
		_st77:
		if ( p == eof )
			goto _out77;
		p+= 1;
		st_case_77:
		if ( p == pe && p != eof )
			goto _out77;
		if ( p == eof ) {
			goto _st77;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr91;
				}
				case 32: {
					goto _ctr95;
				}
				case 45: {
					goto _ctr96;
				}
				case 61: {
					goto _ctr97;
				}
				case 92: {
					goto _ctr98;
				}
				case 95: {
					goto _ctr96;
				}
				case 109: {
					goto _ctr132;
				}
			}
			if ( ( (*( p))) < 9 ) {
				if ( ( (*( p))) < -32 ) {
					if ( -59 <= ( (*( p))) ) {
						goto _ctr92;
					}
				} else if ( ( (*( p))) > -17 ) {
					if ( ( (*( p))) <= -12 ) {
						goto _ctr94;
					}
				} else {
					goto _ctr93;
				}
			} else if ( ( (*( p))) > 13 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr96;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr96;
					}
				} else {
					goto _ctr96;
				}
			} else {
				goto _ctr95;
			}
			goto _ctr90;
		}
		_ctr132:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 5367 "hpricot_css.c"

		goto _st78;
		_st78:
		if ( p == eof )
			goto _out78;
		p+= 1;
		st_case_78:
		if ( p == pe && p != eof )
			goto _out78;
		if ( p == eof ) {
			goto _st78;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr91;
				}
				case 32: {
					goto _ctr95;
				}
				case 45: {
					goto _ctr96;
				}
				case 61: {
					goto _ctr97;
				}
				case 92: {
					goto _ctr98;
				}
				case 95: {
					goto _ctr96;
				}
				case 101: {
					goto _ctr134;
				}
			}
			if ( ( (*( p))) < 9 ) {
				if ( ( (*( p))) < -32 ) {
					if ( -59 <= ( (*( p))) ) {
						goto _ctr92;
					}
				} else if ( ( (*( p))) > -17 ) {
					if ( ( (*( p))) <= -12 ) {
						goto _ctr94;
					}
				} else {
					goto _ctr93;
				}
			} else if ( ( (*( p))) > 13 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr96;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr96;
					}
				} else {
					goto _ctr96;
				}
			} else {
				goto _ctr95;
			}
			goto _ctr90;
		}
		_ctr134:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 5439 "hpricot_css.c"

		goto _st79;
		_st79:
		if ( p == eof )
			goto _out79;
		p+= 1;
		st_case_79:
		if ( p == pe && p != eof )
			goto _out79;
		if ( p == eof ) {
			goto _st79;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr91;
				}
				case 32: {
					goto _ctr95;
				}
				case 45: {
					goto _ctr96;
				}
				case 61: {
					goto _ctr136;
				}
				case 92: {
					goto _ctr98;
				}
				case 95: {
					goto _ctr96;
				}
			}
			if ( ( (*( p))) < 9 ) {
				if ( ( (*( p))) < -32 ) {
					if ( -59 <= ( (*( p))) ) {
						goto _ctr92;
					}
				} else if ( ( (*( p))) > -17 ) {
					if ( ( (*( p))) <= -12 ) {
						goto _ctr94;
					}
				} else {
					goto _ctr93;
				}
			} else if ( ( (*( p))) > 13 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr96;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr96;
					}
				} else {
					goto _ctr96;
				}
			} else {
				goto _ctr95;
			}
			goto _ctr90;
		}
		_ctr136:
			{
#line 39 "hpricot_css.rl"
			
			ape = p;
			aps2 = p;
		}
		
#line 5508 "hpricot_css.c"

		goto _st80;
		_st80:
		if ( p == eof )
			goto _out80;
		p+= 1;
		st_case_80:
		if ( p == pe && p != eof )
			goto _out80;
		if ( p == eof ) {
			goto _st80;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _ctr138;
				}
				case 32: {
					goto _ctr102;
				}
				case 34: {
					goto _ctr103;
				}
				case 39: {
					goto _ctr104;
				}
				case 45: {
					goto _ctr142;
				}
				case 61: {
					goto _ctr124;
				}
				case 92: {
					goto _ctr143;
				}
				case 93: {
					goto _st0;
				}
				case 95: {
					goto _ctr142;
				}
			}
			if ( ( (*( p))) < 9 ) {
				if ( ( (*( p))) < -32 ) {
					if ( -59 <= ( (*( p))) ) {
						goto _ctr139;
					}
				} else if ( ( (*( p))) > -17 ) {
					if ( ( (*( p))) <= -12 ) {
						goto _ctr141;
					}
				} else {
					goto _ctr140;
				}
			} else if ( ( (*( p))) > 13 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr142;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr142;
					}
				} else {
					goto _ctr142;
				}
			} else {
				goto _ctr102;
			}
			goto _ctr101;
		}
		_ctr138:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5585 "hpricot_css.c"

			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 5594 "hpricot_css.c"

		goto _st81;
		_st81:
		if ( p == eof )
			goto _out81;
		p+= 1;
		st_case_81:
		if ( p == pe && p != eof )
			goto _out81;
		if ( p == eof ) {
			goto _st81;}
		else {
			if ( ( (*( p))) == 93 ) {
				goto _ctr106;
			}
			if ( -88 <= ( (*( p))) && ( (*( p))) <= -65 ) {
				goto _st82;
			}
			goto _st57;
		}
		_ctr142:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5621 "hpricot_css.c"

			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 5630 "hpricot_css.c"

		goto _st82;
		_st82:
		if ( p == eof )
			goto _out82;
		p+= 1;
		st_case_82:
		if ( p == pe && p != eof )
			goto _out82;
		if ( p == eof ) {
			goto _st82;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st81;
				}
				case 45: {
					goto _st82;
				}
				case 92: {
					goto _st86;
				}
				case 93: {
					goto _ctr150;
				}
				case 95: {
					goto _st82;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st84;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st83;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _st82;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st82;
					}
				} else {
					goto _st82;
				}
			} else {
				goto _st85;
			}
			goto _st57;
		}
		_ctr139:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5691 "hpricot_css.c"

			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 5700 "hpricot_css.c"

		goto _st83;
		_st83:
		if ( p == eof )
			goto _out83;
		p+= 1;
		st_case_83:
		if ( p == pe && p != eof )
			goto _out83;
		if ( p == eof ) {
			goto _st83;}
		else {
			if ( ( (*( p))) == 93 ) {
				goto _ctr106;
			}
			if ( ( (*( p))) <= -65 ) {
				goto _st82;
			}
			goto _st57;
		}
		_ctr140:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5727 "hpricot_css.c"

			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 5736 "hpricot_css.c"

		goto _st84;
		_st84:
		if ( p == eof )
			goto _out84;
		p+= 1;
		st_case_84:
		if ( p == pe && p != eof )
			goto _out84;
		if ( p == eof ) {
			goto _st84;}
		else {
			if ( ( (*( p))) == 93 ) {
				goto _ctr106;
			}
			if ( ( (*( p))) <= -65 ) {
				goto _st83;
			}
			goto _st57;
		}
		_ctr141:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5763 "hpricot_css.c"

			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 5772 "hpricot_css.c"

		goto _st85;
		_st85:
		if ( p == eof )
			goto _out85;
		p+= 1;
		st_case_85:
		if ( p == pe && p != eof )
			goto _out85;
		if ( p == eof ) {
			goto _st85;}
		else {
			if ( ( (*( p))) == 93 ) {
				goto _ctr106;
			}
			if ( ( (*( p))) <= -65 ) {
				goto _st84;
			}
			goto _st57;
		}
		_ctr143:
			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5799 "hpricot_css.c"

			{
#line 44 "hpricot_css.rl"
			
			ape2 = p;
			PUSH(aps, ape);
			PUSH(aps2, ape2);
		}
		
#line 5808 "hpricot_css.c"

		goto _st86;
		_st86:
		if ( p == eof )
			goto _out86;
		p+= 1;
		st_case_86:
		if ( p == pe && p != eof )
			goto _out86;
		if ( p == eof ) {
			goto _st86;}
		else {
			switch( ( (*( p))) ) {
				case 46: {
					goto _st82;
				}
				case 93: {
					goto _ctr106;
				}
			}
			goto _st57;
		}
		_ctr162:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 5835 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 5842 "hpricot_css.c"

			{
#line 81 "hpricot_css.rl"
			{act = 5;}}
		
#line 5847 "hpricot_css.c"

		goto _st122;
		_st122:
		if ( p == eof )
			goto _out122;
		p+= 1;
		st_case_122:
		if ( p == pe && p != eof )
			goto _out122;
		if ( p == eof ) {
			goto _ctr164;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st1;
				}
				case 45: {
					goto _ctr2;
				}
				case 92: {
					goto _st5;
				}
				case 95: {
					goto _ctr2;
				}
				case 118: {
					goto _ctr216;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st3;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st2;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr2;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr2;
					}
				} else {
					goto _ctr2;
				}
			} else {
				goto _st4;
			}
			goto _ctr164;
		}
		_ctr216:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 5906 "hpricot_css.c"

			{
#line 81 "hpricot_css.rl"
			{act = 5;}}
		
#line 5911 "hpricot_css.c"

		goto _st123;
		_st123:
		if ( p == eof )
			goto _out123;
		p+= 1;
		st_case_123:
		if ( p == pe && p != eof )
			goto _out123;
		if ( p == eof ) {
			goto _ctr164;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st1;
				}
				case 45: {
					goto _ctr2;
				}
				case 92: {
					goto _st5;
				}
				case 95: {
					goto _ctr2;
				}
				case 101: {
					goto _ctr217;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st3;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st2;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr2;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr2;
					}
				} else {
					goto _ctr2;
				}
			} else {
				goto _st4;
			}
			goto _ctr164;
		}
		_ctr217:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 5970 "hpricot_css.c"

			{
#line 81 "hpricot_css.rl"
			{act = 5;}}
		
#line 5975 "hpricot_css.c"

		goto _st124;
		_st124:
		if ( p == eof )
			goto _out124;
		p+= 1;
		st_case_124:
		if ( p == pe && p != eof )
			goto _out124;
		if ( p == eof ) {
			goto _ctr164;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st1;
				}
				case 45: {
					goto _ctr2;
				}
				case 92: {
					goto _st5;
				}
				case 95: {
					goto _ctr2;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st3;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st2;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr2;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr2;
					}
				} else {
					goto _ctr2;
				}
			} else {
				goto _st4;
			}
			goto _ctr164;
		}
		_ctr163:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 6031 "hpricot_css.c"

			{
#line 30 "hpricot_css.rl"
			
			aps = p;
		}
		
#line 6038 "hpricot_css.c"

			{
#line 81 "hpricot_css.rl"
			{act = 5;}}
		
#line 6043 "hpricot_css.c"

		goto _st125;
		_st125:
		if ( p == eof )
			goto _out125;
		p+= 1;
		st_case_125:
		if ( p == pe && p != eof )
			goto _out125;
		if ( p == eof ) {
			goto _ctr164;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st1;
				}
				case 45: {
					goto _ctr2;
				}
				case 92: {
					goto _st5;
				}
				case 95: {
					goto _ctr2;
				}
				case 100: {
					goto _ctr218;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st3;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st2;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr2;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr2;
					}
				} else {
					goto _ctr2;
				}
			} else {
				goto _st4;
			}
			goto _ctr164;
		}
		_ctr218:
			{
#line 1 "NONE"
			{te = p+1;}}
		
#line 6102 "hpricot_css.c"

			{
#line 81 "hpricot_css.rl"
			{act = 5;}}
		
#line 6107 "hpricot_css.c"

		goto _st126;
		_st126:
		if ( p == eof )
			goto _out126;
		p+= 1;
		st_case_126:
		if ( p == pe && p != eof )
			goto _out126;
		if ( p == eof ) {
			goto _ctr164;}
		else {
			switch( ( (*( p))) ) {
				case -60: {
					goto _st1;
				}
				case 45: {
					goto _ctr2;
				}
				case 92: {
					goto _st5;
				}
				case 95: {
					goto _ctr2;
				}
			}
			if ( ( (*( p))) < -16 ) {
				if ( ( (*( p))) > -33 ) {
					{
						goto _st3;
					}
				} else if ( ( (*( p))) >= -59 ) {
					goto _st2;
				}
			} else if ( ( (*( p))) > -12 ) {
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr2;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr2;
					}
				} else {
					goto _ctr2;
				}
			} else {
				goto _st4;
			}
			goto _ctr164;
		}
		_out87: cs = 87; goto _out; 
		_out0: cs = 0; goto _out; 
		_out1: cs = 1; goto _out; 
		_out88: cs = 88; goto _out; 
		_out2: cs = 2; goto _out; 
		_out3: cs = 3; goto _out; 
		_out4: cs = 4; goto _out; 
		_out5: cs = 5; goto _out; 
		_out89: cs = 89; goto _out; 
		_out6: cs = 6; goto _out; 
		_out90: cs = 90; goto _out; 
		_out7: cs = 7; goto _out; 
		_out8: cs = 8; goto _out; 
		_out91: cs = 91; goto _out; 
		_out9: cs = 9; goto _out; 
		_out10: cs = 10; goto _out; 
		_out11: cs = 11; goto _out; 
		_out12: cs = 12; goto _out; 
		_out92: cs = 92; goto _out; 
		_out93: cs = 93; goto _out; 
		_out13: cs = 13; goto _out; 
		_out14: cs = 14; goto _out; 
		_out94: cs = 94; goto _out; 
		_out15: cs = 15; goto _out; 
		_out16: cs = 16; goto _out; 
		_out17: cs = 17; goto _out; 
		_out18: cs = 18; goto _out; 
		_out19: cs = 19; goto _out; 
		_out20: cs = 20; goto _out; 
		_out95: cs = 95; goto _out; 
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
		_out40: cs = 40; goto _out; 
		_out41: cs = 41; goto _out; 
		_out96: cs = 96; goto _out; 
		_out97: cs = 97; goto _out; 
		_out42: cs = 42; goto _out; 
		_out43: cs = 43; goto _out; 
		_out98: cs = 98; goto _out; 
		_out99: cs = 99; goto _out; 
		_out100: cs = 100; goto _out; 
		_out101: cs = 101; goto _out; 
		_out102: cs = 102; goto _out; 
		_out103: cs = 103; goto _out; 
		_out104: cs = 104; goto _out; 
		_out105: cs = 105; goto _out; 
		_out106: cs = 106; goto _out; 
		_out107: cs = 107; goto _out; 
		_out108: cs = 108; goto _out; 
		_out109: cs = 109; goto _out; 
		_out110: cs = 110; goto _out; 
		_out44: cs = 44; goto _out; 
		_out45: cs = 45; goto _out; 
		_out46: cs = 46; goto _out; 
		_out47: cs = 47; goto _out; 
		_out48: cs = 48; goto _out; 
		_out49: cs = 49; goto _out; 
		_out50: cs = 50; goto _out; 
		_out51: cs = 51; goto _out; 
		_out111: cs = 111; goto _out; 
		_out112: cs = 112; goto _out; 
		_out113: cs = 113; goto _out; 
		_out114: cs = 114; goto _out; 
		_out115: cs = 115; goto _out; 
		_out116: cs = 116; goto _out; 
		_out117: cs = 117; goto _out; 
		_out118: cs = 118; goto _out; 
		_out119: cs = 119; goto _out; 
		_out52: cs = 52; goto _out; 
		_out53: cs = 53; goto _out; 
		_out54: cs = 54; goto _out; 
		_out55: cs = 55; goto _out; 
		_out56: cs = 56; goto _out; 
		_out57: cs = 57; goto _out; 
		_out58: cs = 58; goto _out; 
		_out59: cs = 59; goto _out; 
		_out120: cs = 120; goto _out; 
		_out60: cs = 60; goto _out; 
		_out61: cs = 61; goto _out; 
		_out62: cs = 62; goto _out; 
		_out121: cs = 121; goto _out; 
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
		_out84: cs = 84; goto _out; 
		_out85: cs = 85; goto _out; 
		_out86: cs = 86; goto _out; 
		_out122: cs = 122; goto _out; 
		_out123: cs = 123; goto _out; 
		_out124: cs = 124; goto _out; 
		_out125: cs = 125; goto _out; 
		_out126: cs = 126; goto _out; 
		_out: {}
	}
	
#line 115 "hpricot_css.rl"

	
	rb_gc_unregister_address(&focus);
	rb_gc_unregister_address(&tmpt);
	return focus;
}
