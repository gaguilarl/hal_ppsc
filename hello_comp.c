/*
 * hello_comp.c
 *
 *  Created on: 15/06/2017
 *      Author: gabriel
 */

#include <rtapi_slab.h>		/* kmalloc() */
#include <rtapi_io.h>		/* kmalloc() */
#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */
#include "hal.h"		/* HAL public API decls */
//#include "hal_parport.h"

MODULE_LICENSE("GPL");

static void toggle_val(void *arg, long period);

static int comp_id;

typedef struct {
	hal_bit_t *bit_out;
	hal_bit_t invert_out;
}test_out_t;

int rtapi_app_main(void){
	int retval;
	test_out_t *digout = hal_malloc(sizeof(test_out_t));
	if(digout == 0)
	{
		rtapi_print_msg(RTAPI_MSG_ERR, "Memory alocation failed\n Exiting\n");
		return -1;
	}
    comp_id = hal_init("hello_comp");
    rtapi_set_msg_level(RTAPI_MSG_ERR);
    if (comp_id < 0) {
	rtapi_print_msg(RTAPI_MSG_ERR, "Component not initialized, be sad.\n");
		return -1;
    }
    rtapi_print_msg(RTAPI_MSG_ERR, "Hello HAL <3 \n");
    retval = hal_pin_bit_new("hello_comp.bit.out", HAL_OUT, &(digout->bit_out), comp_id);
    if (retval != 0) {
    	return retval;
    }
	hal_export_funct("hello_comp.change-val", toggle_val, &(digout),0, 0, comp_id);
    hal_ready(comp_id);
    return 0;
}
void rtapi_app_exit(void)
{
	rtapi_print_msg(RTAPI_MSG_ERR, "Bye, bye, HAL <3.\n");
    hal_exit(comp_id);
}

static void toggle_val(void *arg, long period){
	//bus_data_t *bus;
	//bus = *(bus_data_t **)(arg);
	test_out_t *funct_arg = arg;
	hal_bit_t act_val = *(funct_arg->bit_out);
	act_val ^= 1;
	rtapi_print_msg(RTAPI_MSG_ERR, "Bit %x\n",act_val);
}
