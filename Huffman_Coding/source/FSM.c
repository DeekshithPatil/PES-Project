/*
 * FSM.c - Contains functions definitions necessary to implement the FSM needed for Assignment-6
 *
 *  Created on: Nov 8, 2021
 *      Author: Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
 *
 *  Implementation credits: Howdy Pierce, Class lecture
 */


#include "sysclock.h"
#include "UART.h"
#include "cbfifo.h"
#include "syscalls.h"
#include "FSM.h"
#include <stdbool.h>
#include <string.h>

static void handle_author(int argc, char* argv[]);
static void handle_dump(int argc, char* argv[]);
static void handle_help(int argc, char* argv[]);
static void handle_info(int argc, char* argv[]);

#define	MAX_HEXDUMP_BYTES			640
#define LOWER_16BITS_MASK			0x0000FFFF
#define	UPPER_TO_LOWER_16_SHIFT		16

/****************************** Command Table Section ****************************************/

typedef void (*command_handler_t)(int, char *argv[]);

typedef struct {
  const char *name;
  command_handler_t handler;
  const char *help_string;
} command_table_t;

static const command_table_t commands[] = {
	/* Author */ {"Author",handle_author,"\r\n1. ""Author"" - Prints the author of the code\r\n"},
	/* Dump   */ {"Dump",handle_dump,"\r\n2. ""Dump"" <start (in hex)> <len (in decimal or hex)> - Performs hexdump\r\n"},
	/* Help   */ {"Help",handle_help,"\r\n3. ""Help"" - Prints all supported commands\r\n"},
	/* Info   */ {"Info",handle_info,"\r\n4. ""Info"" - Prints build information\r\n"}
};

static const int num_commands =
  sizeof(commands) / sizeof(command_table_t);



/************************************************************************************************/



static void handle_command(int argc, char* argv[]);

//Refer FSM.h for function description
void accumulate_line(char *str)
{
	int str_index = 0;
	char ch;

	while(1)
	{
//		while(cbfifo_length(Rx_Buffer) == 0){} //Wait as long as receive buffer is empty

		ch = getchar();

		if(ch == '\n' || ch == '\r') //if enter button was pressed
		{
			str[str_index] = '\0';
			return;
		}

		if(ch == '\b' || ch == 127) //if backspace was encountered
		{
			if(str_index == 0)
			{
				goto NEXT;
			}
			str_index--;
			putchar('\b');
			putchar(' ');
			putchar('\b');
			goto NEXT;
		}

		else if(ch == ' ') //in case of space, inser '_'
		{
			putchar(ch);
			ch = '_';
			str[str_index++] = ch;
			goto NEXT;
		}

		str[str_index++] = ch;

		putchar(ch);



		if(str_index >= CAPACITY)
		{
			str_index = 0;
		}

		// start transmitter if it isn't already running
NEXT:		if (!(UART0->C2 & UART0_C2_TIE_MASK)) {
			UART0->C2 |= UART0_C2_TIE(1);
		}
	}

}

//Refer FSM.h for function description
void process_command(char * str)
{
//	printf("\r\nReceived string is : %s\r\n",str);


	char *p = str;

	char *end = NULL;

	// find end of string
	for(end = str;*end!='\0';end++)
	{
		//do nothing
	}

	//Tokenize input in place
	bool in_token = false;

	char *argv[10];

	int argc = 0;

	memset(argv, 0, sizeof(argv));

	for(p = str; p < end; p++)
	{
		//If we are outside token,check until we encounter a '_'
		if(in_token == false)
		{
			if(*p != '_') //if '_' is encountered, change state
			{
				argv[argc++] = p;
				in_token = true;
			}

			else
			{
				//do nothing
			}
		}
		//process a character while "in_token"
		else
		{
			if(*p == '_')
			{
				*p = '\0';
				in_token = false;
			}
		}
	}

	argv[argc] = NULL;

	if(argc == 0) //no command
		return;

	handle_command(argc, argv);

}


/*
 * Breif: This function is used to loop through the tokens and check if any command exists.
 * 		  If a command does exist, then call the appropriate call back function
 */
static void handle_command(int argc, char* argv[])
{

	for(int i=0; i < num_commands; i++)
	{
	    if (strcasecmp(argv[0], commands[i].name) == 0) //check if argv[0] matches with any "Name" field in command table
	    {
	      commands[i].handler(argc, argv);
	      return;
	    }

	}

	printf("\r\nUnknown command: %s\r\n",argv[0]);
}


/*
 * Brief - Handler function for the command "Author"
 */
static void handle_author(int argc, char* argv[])
{
	if(argc!=1)
	{
		printf("\r\nSorry! Author command does not take any other parameter\r\n");
		return;
	}

	printf("\r\nDeekshith Reddy Patil\r\n");

}

/*
 * Brief - Handler function for the command "Dump"
 */
static void handle_dump(int argc, char* argv[])
{

	unsigned int start_address=0;
	unsigned int bytes=0;

	if(argc!=3)
	{
		printf("\r\nSorry! Invalid parameters received for Dump Command!\r\n");
		return;
	}

	sscanf(argv[1],"%x",&start_address);


	if((argv[2][1]=='x' || argv[2][1]=='X')) //Check if user had entered number of bytes in hex
	{
		sscanf(argv[2],"%x",&bytes); //Scan bytes in hex
	}

	else
	{
		sscanf(argv[2],"%u",&bytes); //Scan bytes in decimal
	}

	if(bytes == 0)
	{
		printf("\r\nInvalid Entry for <len>\r\n");
		return;
	}

	if(bytes > MAX_HEXDUMP_BYTES)
	{
		printf("\r\nSorry!Number of bytes requested cannot be greater than 640\r\n");
		return;
	}

	printf("\r\n");
	for(int i=0;i<bytes;i++)
	{
		if((i & (UPPER_TO_LOWER_16_SHIFT-1)) == 0) //i.e i%16
		{

			printf("\r\n%04x_%04x  ",((start_address + i) >> UPPER_TO_LOWER_16_SHIFT)& LOWER_16BITS_MASK,(start_address + i)&LOWER_16BITS_MASK);
		}

		printf("%02x ",*(uint8_t *)(start_address + i));
	}


	printf("\r\n");
}

/*
 * Brief - Handler function for the command "Help"
 */
static void handle_help(int argc, char* argv[])
{
	if(argc!=1)
	{
		printf("\r\nSorry! Help command does not take any other parameter\r\n");
		return;
	}

	for(int i=0; i < num_commands; i++)
	{

	    printf("%s",commands[i].help_string);

	}


}

/*
 * Brief - Handler function for the command "Info"
 */
static void handle_info(int argc, char* argv[])
{
	if(argc!=1)
	{
		printf("\r\nSorry! Info command does not take any other parameter\r\n");
		return;
	}

	printf("Hello world!\r\n");
}
