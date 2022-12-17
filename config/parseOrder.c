
#line 1 ".\\parseOrder.rl"
/*
 * Convert a string to an integer.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>


#line 11 ".\\parseOrder.c"
static const char _parse_plc_resp_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4
};

static const char _parse_plc_resp_key_offsets[] = {
	0, 0, 2, 5, 10, 15, 17, 20, 
	21, 22, 28, 30, 33, 34, 35, 36
};

static const char _parse_plc_resp_trans_keys[] = {
	48, 57, 46, 48, 57, 32, 65, 90, 
	97, 122, 96, 65, 90, 97, 122, 48, 
	57, -27, 48, 57, -123, -125, 32, 61, 
	65, 90, 97, 122, 32, 61, 32, 48, 
	57, -27, -123, -125, 0
};

static const char _parse_plc_resp_single_lengths[] = {
	0, 0, 1, 1, 1, 0, 1, 1, 
	1, 2, 2, 1, 1, 1, 1, 0
};

static const char _parse_plc_resp_range_lengths[] = {
	0, 1, 1, 2, 2, 1, 1, 0, 
	0, 2, 0, 1, 0, 0, 0, 0
};

static const char _parse_plc_resp_index_offsets[] = {
	0, 0, 2, 5, 9, 13, 15, 18, 
	20, 22, 27, 30, 33, 35, 37, 39
};

static const char _parse_plc_resp_indicies[] = {
	0, 1, 2, 0, 1, 2, 3, 3, 
	1, 4, 3, 3, 1, 5, 1, 6, 
	5, 1, 7, 1, 8, 1, 9, 10, 
	3, 3, 1, 9, 10, 1, 10, 11, 
	1, 12, 1, 13, 1, 14, 1, 1, 
	0
};

static const char _parse_plc_resp_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15
};

static const char _parse_plc_resp_trans_actions[] = {
	1, 0, 0, 3, 0, 5, 0, 0, 
	0, 0, 0, 7, 0, 0, 9
};

static const int parse_plc_resp_start = 1;
static const int parse_plc_resp_first_final = 15;
static const int parse_plc_resp_error = 0;

static const int parse_plc_resp_en_main = 1;


#line 13 ".\\parseOrder.rl"


enum orderFormat
{
    invalidFormat,
    defaultFormat,
}

typedef struct
{
    char name[128];
    int price;
} o_menu




static int _parse_order_num = 0;
static int _menu_count = 0;
static int _menu_name_index=0;
static o_menu _menus[128];
static int _total_price = 0;

static orderFormat parse_plc_resp_str_( char *str, int len )
{
    bool ret = invalidFormat;

	char *p = str, *pe = str + len;
	int cs;

    _menu_count = 0;
    _menu_name_index = 0;


	
#line 103 ".\\parseOrder.c"
	{
	cs = parse_plc_resp_start;
	}

#line 106 ".\\parseOrder.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _parse_plc_resp_trans_keys + _parse_plc_resp_key_offsets[cs];
	_trans = _parse_plc_resp_index_offsets[cs];

	_klen = _parse_plc_resp_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _parse_plc_resp_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _parse_plc_resp_indicies[_trans];
	cs = _parse_plc_resp_trans_targs[_trans];

	if ( _parse_plc_resp_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _parse_plc_resp_actions + _parse_plc_resp_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 48 ".\\parseOrder.rl"
	{
            _parse_order_num = _parse_order_num * 10 + ((*p) - '0');
        }
	break;
	case 1:
#line 57 ".\\parseOrder.rl"
	{
            _menus[_menu_count].name[_menu_name_index++] = (*p);
        }
	break;
	case 2:
#line 61 ".\\parseOrder.rl"
	{
            _menus[_menu_count].price = _menus[_menu_count].price * 10 + ((*p) - '0');
        }
	break;
	case 3:
#line 65 ".\\parseOrder.rl"
	{
            _total_price = _total_price * 10 + ((*p) - '0');
        }
	break;
	case 4:
#line 74 ".\\parseOrder.rl"
	{ret = defaultFormat;}
	break;
#line 202 ".\\parseOrder.c"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 81 ".\\parseOrder.rl"

	if ( cs < parse_plc_resp_first_final ){
        char ch[512];
        memset(ch,0,sizeof(ch));
        if(len>0)
        {
            memcpy(ch, str, len);
        }
		fprintf( stderr, "parse_plc_resp_str_: there was an error,cs:[%d],str:[%s]\n", cs, ch);
    }

	return ret;
};
