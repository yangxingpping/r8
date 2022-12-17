/*
 * Convert a string to an integer.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

%%{
	machine parse_plc_resp;
	write data;
}%%

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


	%%{
        action add_order_num{
            _parse_order_num = _parse_order_num * 10 + (fc - '0');
            printf("add order part %c, current num %d\n", fc, _parse_order_num);
        }

        action add_menu_count{
            ++_menu_count;
            _menu_name_index = 0;
        }

        action append_menu_name{
            _menus[_menu_count].name[_menu_name_index++] = fc;
            printf("add menu name part %c, current menu name %s\n", fc, _menus[_menu_count].name);
        }

        action append_menu_price{
            _menus[_menu_count].price = _menus[_menu_count].price * 10 + (fc - '0');
            printf("add menu price part %c, current menu price %d\n", fc, _menus[_menu_count].price);
        }

        action add_total_price{
            _total_price = _total_price * 10 + (fc - '0');
            printf("add total price part %c, current total price %d\n", fc, _total_price);
        }

        o_num_frag = (digit @add_order_num)+ ;
        m_item_frag = (alpha @append_menu_name)+'`'(digit @append_menu_price)+'y';

        total_price_frag = (digit @add_total_price)+'y';

        default_format_order = ((' ')* o_num_frag '.' (' ')* (m_item_frag @add_menu_count)(' ')* ( '+' (' ')* (m_item_frag @add_menu_count)(' ')* )*   '='(' ')*total_price_frag@{ret = orderFormat::defaultFormat;});
		main := 
			(default_format_order )
			;
		# Initialize and execute.
		write init;
		write exec;
	}%%
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
