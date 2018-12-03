#include <linux/init.h>
#include <linux/module.h>

// step 1
#include <linux/moduleparam.h>


//step 2 create variable

//int param_var[3]={0,1,2};
//module_param(param_var, int, S_IRUSR | S_IWUSR);
//step 3 register (macro)
//module_param(name_var, type, permissions)
/*
	S_IRUSR
	S_IWUSR
	S_IXUSR
	S_IWGRP
	S_IRGRP
	S_IRUSR | S_IWUSR	
*/


//create variable
int param_var[3] = {10,11,12};

//step 3.2 register (macro)
module_param_array(param_var, int, NULL, S_IRUSR|S_IWUSR);


void display(void){
	printk(KERN_ALERT "Test: param = %d", param_var[0]);
	printk(KERN_ALERT "Test: param = %d", param_var[1]);
	printk(KERN_ALERT "Test: param = %d", param_var[2]);
}


static int hello_init(void){
	printk(KERN_ALERT "Test: Hello world, this is soliduscode\n");
	display();
	return 0;
}


static void hello_exit(void){
	printk(KERN_ALERT "Test: Goodbye, this message is from soliduscode\n");
}


module_init(hello_init);
module_exit(hello_exit);

