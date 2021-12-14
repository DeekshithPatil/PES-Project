/*
 * main.c - application entry point
 *
 * Author Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
 */
#include "sysclock.h"
#include "UART.h"
#include "cbfifo.h"
#include "syscalls.h"
#include "test_cbfifo.h"
#include "Delay.h"

void test_huffman();

int main(void)
{

	//Test the functionality of cbfifo
  	test_cbfifo();

	sysclock_init();

	Init_UART0();

	//test the functionality of huffman
	test_huffman();

	int i = 0;

	while (1)
	{
		//Print the value of i and increment, every 1 second
		printf("%d\n",i++);
		delay(1000);
	}

  return 0 ;
}

/*
 * Brief - This function sends out test strings in order to test the functionality of huffman encoding and decoding
 *
 * Note - Do not Modify the test strings. If modified, the same modifications need to made on the PC side as well
 */
void test_huffman()
{
	printf("$");
	delay(50);

	printf("Hello!");
	delay(50);

	printf("0123456789");
	delay(50);

	printf("12poijadsuf12e90]d'api'af");
	delay(50);

	printf("q[wieofha;kdf;nvajlhaf;dhiulah124y3ro7halbvlajdf;h72rlfah");
	delay(50);

	printf("\nHey! How's it going\n This is a huge string test. Let's see if it works!!!. I'm so excited!!\n");
	delay(50);

	printf("The second string is long as well! This is decoded perfectly as well!!\n");
	delay(50);

	for(int i= 0; i< 3; i++)
	{
		printf("\nDynamic string No. = %d",i+1);
		delay(50);
	}

	printf("Nov 29 00:24:19 deekshith systemd logind[773]: Lid closed.");
	delay(50);

	printf("Nov 29 14:27:52 deekshith sudo: pam_unix(sudo:session): session opened for user root by (uid=0)");
	delay(50);

	printf("base passwd depends on libdebconfclient0 (>= 0.145); however:");
	delay(50);

	printf("(Reading database ... 55");
	delay(50);

	printf("(Reading database ... 220669 files and directories currently installed.)");
	delay(50);

	printf("dbus daemon[930]: [session uid=999 pid=930] Successfully activated service 'org.gtk.vfs.Metadata");
	delay(50);

	printf("Nov 29 00:24:06 deekshith systemd[16697]: Stopped target GNOME Shell on X11.");
	delay(50);

	printf("Nov 29 00:24:06 deekshith gnome shell[16940]: message repeated 2 times: [ == Stack trace for context 0x559b7d3a4900 ==]");
	delay(50);

	printf("Nov 29 00:24:33 deekshith NetworkManager[758]: <info>  [1638170673.6591] dhcp6 (wlp2s0): state changed bound > done");
	delay(50);

	printf("Nov 29 13:17:11 deekshith /usr/lib/gdm3/gdm x session[36856]: (II) xfree86: Adding drm device (/dev/dri/card1)");
	delay(50);

	printf("Nov 25 17:28:02 deekshith kernel: [65826.859822] OOM killer disabled.");
	delay(50);

	printf("Nov 26 13:32:44 deekshith kernel: [65896.363271] sd 0:0:0:0: [sda] Stopping disk");
	delay(50);

	printf("Nov 26 13:35:17 deekshith kernel: [    0.000000] reserve setup_data: [mem 0x0000000100000000 0x000000027f7fffff] usable");
	delay(50);

	delay(1000);
}

