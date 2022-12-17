
#line 1 ".\\parseOrder.rl"
/*
 * Convert a string to an integer.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>


#line 11 ".\\parseOrder.h"
static const char _parse_plc_resp_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5
};

static const char _parse_plc_resp_key_offsets[] = {
	0, 0, 3, 6, 11, 16, 18, 21, 
	24, 27, 30
};

static const char _parse_plc_resp_trans_keys[] = {
	32, 48, 57, 46, 48, 57, 32, 65, 
	90, 97, 122, 96, 65, 90, 97, 122, 
	48, 57, 121, 48, 57, 32, 43, 61, 
	32, 48, 57, 121, 48, 57, 0
};

static const char _parse_plc_resp_single_lengths[] = {
	0, 1, 1, 1, 1, 0, 1, 3, 
	1, 1, 0
};

static const char _parse_plc_resp_range_lengths[] = {
	0, 1, 1, 2, 2, 1, 1, 0, 
	1, 1, 0
};

static const char _parse_plc_resp_index_offsets[] = {
	0, 0, 3, 6, 10, 14, 16, 19, 
	23, 26, 29
};

static const char _parse_plc_resp_indicies[] = {
	0, 2, 1, 3, 2, 1, 3, 4, 
	4, 1, 5, 4, 4, 1, 6, 1, 
	7, 6, 1, 8, 3, 9, 1, 9, 
	10, 1, 11, 10, 1, 1, 0
};

static const char _parse_plc_resp_trans_targs[] = {
	1, 0, 2, 3, 4, 5, 6, 7, 
	7, 8, 9, 10
};

static const char _parse_plc_resp_trans_actions[] = {
	0, 0, 1, 0, 5, 0, 7, 3, 
	0, 0, 9, 11
};

static const int parse_plc_resp_start = 1;
static const int parse_plc_resp_first_final = 10;
static const int parse_plc_resp_error = 0;

static const int parse_plc_resp_en_main = 1;


#line 13 ".\\parseOrder.rl"


enum class orderFormat
{
    invalidFormat,
    defaultFormat,
};

typedef struct
{
    char name[128];
    int price;
} o_menu;




static int _parse_order_num = 0;
static int _menu_count = 0;
static int _menu_name_index=0;
static o_menu _menus[128];
static int _total_price = 0;


static orderFormat parse_plc_resp_str_( char *str, int len )
{
    orderFormat ret = orderFormat::invalidFormat;

	char *p = str, *pe = str + len;
	int cs;

    _menu_count = 0;
    _menu_name_index = 0;
    memset(_menus, 0, sizeof(_menus));


	
#line 102 ".\\parseOrder.h"
	{
	cs = parse_plc_resp_start;
	}

#line 105 ".\\parseOrder.h"
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
#line 50 ".\\parseOrder.rl"
	{
            _parse_order_num = _parse_order_num * 10 + ((*p) - '0');
            printf("add order part %c, current num %d\n", (*p), _parse_order_num);
        }
	break;
	case 1:
#line 55 ".\\parseOrder.rl"
	{
            ++_menu_count;
            _menu_name_index = 0;
        }
	break;
	case 2:
#line 60 ".\\parseOrder.rl"
	{
            _menus[_menu_count].name[_menu_name_index++] = (*p);
            printf("add menu name part %c, current menu name %s\n", (*p), _menus[_menu_count].name);
        }
	break;
	case 3:
#line 65 ".\\parseOrder.rl"
	{
            _menus[_menu_count].price = _menus[_menu_count].price * 10 + ((*p) - '0');
            printf("add menu price part %c, current menu price %d\n", (*p), _menus[_menu_count].price);
        }
	break;
	case 4:
#line 70 ".\\parseOrder.rl"
	{
            _total_price = _total_price * 10 + ((*p) - '0');
            printf("add total price part %c, current total price %d\n", (*p), _total_price);
        }
	break;
	case 5:
#line 80 ".\\parseOrder.rl"
	{ret = orderFormat::defaultFormat;}
	break;
#line 211 ".\\parseOrder.h"
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

#line 87 ".\\parseOrder.rl"

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
