

#ifndef QUADROTOR_CURRENT_WORK_SERIAL_H_
#define QUADROTOR_CURRENT_WORK_SERIAL_H_

//#define USE_FIFO_UART_


//#define UART_PC_ 	UART0_BASE // define uart module used in this program for communication
//#define UART_BASE_KINECT	UART6_BASE
#define UART_COM_2_CONTROLLER_		UART0_BASE
//#define UART_RS485_3_		UART3_BASE


extern float data_buffer_cn[10];


#define STR_TERMINATOR_ '\0' //string from MCU
#define COM_TERMINATOR_ '\n' // string from computer

#define CN_1_ '~'
#define CN_2_ '!'
#define CN_3_ '@'
#define CN_4_ '#'
#define CN_5_ '$'
#define CN_6_ '%'
#define CN_7_ '^'
#define CN_8_ '*'
#define CN_9_ '('
#define CN_10_ ')'


typedef struct {
	volatile unsigned char Flag_receive;
	char Command_Data[40];
//	 char TX_software_FIFO[100];
} MyUart;

extern MyUart UartPC;
extern MyUart Uart;
extern MyUart UartCamera;

extern unsigned int UART5_data;
/*
 * Define the protocol
 */
#define COM2CTL_MOTOR_		'm'
#define TEST_MOTOR_1_			'1'
#define TEST_MOTOR_2_			'2'
#define TEST_MOTOR_3_			'3'
#define TEST_MOTOR_4_			'4'
#define MOTOR_STOP_				'0'

#define COM2CTL_DISPLAY_ON_				'e'
#define COM2CTL_DISPLAY_MODE_0_			   	'0'
#define COM2CTL_DISPLAY_MODE_1_				'1'
#define COM2CTL_DISPLAY_MODE_2_				'2'
#define COM2CTL_DISPLAY_MODE_3_				'3'
#define COM2CTL_DISPLAY_MODE_4_				'4'
#define COM2CTL_DISPLAY_MODE_5_				'5'
#define COM2CTL_DISPLAY_MODE_6_				'6'
#define COM2CTL_DISPLAY_MODE_7_				'7'
#define COM2CTL_DISPLAY_OFF_			'f'

#define COM2CTL_MOTOR_ON_		'c'
#define COM2CTL_MOTOR_OFF_		'd'

#define COM2CTL_AUTONOMOUS_ON_	'q'
#define COM2CTL_AUTONOMOUS_OFF_	'w'

#define COM2CTL_ONLINE_PID_TUNE_ON_		'a'
#define COM2CTL_ONLINE_PID_TUNE_OFF_	'n'

#define COM2CTL_SET_COLOR_RED_			'r'
#define COM2CTL_SET_COLOR_GREEN_		'g'
#define COM2CTL_SET_COLOR_BLUE_			'b'

#define SENSOR2CTL_BLUE_ 	'b'
#define SENSOR2CTL_RED_		'r'
#define SENSOR2CTL_GREEN_	'g'
#define SENSOR2CTL_ERROR_	'e'

#define SENSOR2CTL_X_		'x'
#define SENSOR2CTL_Y_		'y'
#define SENSOR2CTL_SET_COLOR_ERROR_		'l'




#define COM2CTL_SET_KP_ 'P'
#define COM2CTL_SET_KI_ 'I'
#define COM2CTL_SET_KD_ 'D'
#define COM2CTL_SET_SETPOINT_ 'S'

#define COM2CTL_PID_ROLL_    	'R'
#define COM2CTL_PID_PITCH_   	'P'
#define COM2CTL_PID_YAW_       	'W'
#define COM2CTL_PID_X_         	'X'
#define COM2CTL_PID_Y_         	'Y'
#define COM2CTL_PID_Z_        	'Z'

#define COM2CTL_SYSTEM_INFOR_ 'i'



//#define COM2CTL_RUN					42//'*'
//#define COM2CTL_RUN_ON					49//'1'
//#define COM2CTL_RUN_OFF					48//'0'
//
//#define COM2CTL_TEST_MOTOR			33//'!'
//#define COM2CTL_TEST_MOTOR_ON			49//'1'
//#define COM2CTL_TEST_MOTOR_OFF			48//'0'
//#define COM2CTL_TEST_MOTOR_1		34//'"'
//#define COM2CTL_TEST_MOTOR_2		35//'#'
//#define COM2CTL_TEST_MOTOR_3		36//'$'
//#define COM2CTL_TEST_MOTOR_4		37//'%'
//
//#define COM2CTL_DISPLAY				38//'&' reserve 38 to 41
//#define COM2CTL_DISPLAY_ON				49//'1'
//#define COM2CTL_DISPLAY_OFF				48//'0'
//
//#define COM2CTL_MOTOR				39// '''
//#define COM2CTL_MOTOR_ON				49//'1'
//#define COM2CTL_MOTOR_OFF				48//'0'
//
//#define SENSOR2CTL_POSITION			40//'('
//
//#define COM2CTL_SET_PARAMETER		59//';'
//#define COM2CTL_SET_PARAMETER_OFF		48//'0'
//#define	COM2CTL_SET_PARAMETER_ROLL		60//'<' reserve 60 to 64
//#define COM2CTL_SET_PARAMETER_PITCH		61//'='
//#define COM2CTL_SET_PARAMETER_YAW		62//'>'
//#define COM2CTL_SET_PARAMETER_Y			63//'?'
//#define COM2CTL_SET_PARAMETER_X			64//'@'
//#define COM2CTL_SET_PARAMETER_Z			40//'('
//
//
//#define COM2CTL_SCAN_PARAMETER		60//'<'
//#define COM2CTL_SET_KP				61//'='
//#define COM2CTL_SET_KI				62//'>'
//#define COM2CTL_SET_KD				63//'?'
//
//#define CTL2COM_SCAN_PARAMETER		60//'<'
//#define CTL2COM_DISPLAY				38//'&'


// init UART_port
extern void UART0_Init();
extern void UART6_Init();
extern void UART5_Init();
extern void UART1_Init();
extern void UART3_Init_2();

extern void SerialGetStr(uint32_t ui32Base,char *uart_str);

extern void SerialPutData(uint32_t ui32Base,char *uart_data,unsigned char dataSize);

extern void SerialPutChar(uint32_t ui32Base,unsigned char uart_char);
extern void SerialPutStr(uint32_t ui32Base,char *uart_str);
extern void SerialPutStr_NonTer(uint32_t ui32Base,char *uart_str);
extern void SerialPutStrLn(uint32_t ui32Base,char *uart_str);
extern void SerialTerminator(uint32_t ui32Base);
extern void Serial_Sendcommand_Camera(char *uart_str);
extern void set_position (char *string, int16_t *position);
#endif //QUADROTOR_CURRENT_WORK_SERIAL_H_

//======Ascii code==========================
//	binary|demical|hexa|ascii
//	010 0000	32	20	space (sp)
//	010 0001	33	21	!
//	010 0010	34	22	"
//	010 0011	35	23	#
//	010 0100	36	24	$
//	010 0101	37	25	%
//	010 0110	38	26	&
//	010 0111	39	27	'
//	010 1000	40	28	(
//	010 1001	41	29	)
//	010 1010	42	2A	*
//	010 1011	43	2B	+
//	010 1100	44	2C	,
//	010 1101	45	2D	-
//	010 1110	46	2E	.
//	010 1111	47	2F	/
//	011 0000	48	30	0
//	011 0001	49	31	1
//	011 0010	50	32	2
//	011 0011	51	33	3
//	011 0100	52	34	4
//	011 0101	53	35	5
//	011 0110	54	36	6
//	011 0111	55	37	7
//	011 1000	56	38	8
//	011 1001	57	39	9
//	011 1010	58	3A	:
//	011 1011	59	3B	;
//	011 1100	60	3C	<
//	011 1101	61	3D	=
//	011 1110	62	3E	>
//	011 1111	63	3F	?
//	100 0000	64	40	@
//	100 0001	65	41	A
//	100 0010	66	42	B
//	100 0011	67	43	C
//	100 0100	68	44	D
//	100 0101	69	45	E
//	100 0110	70	46	F
//	100 0111	71	47	G
//	100 1000	72	48	H
//	100 1001	73	49	I
//	100 1010	74	4A	J
//	100 1011	75	4B	K
//	100 1100	76	4C	L
//	100 1101	77	4D	M
//	100 1110	78	4E	N
//	100 1111	79	4F	O
//	101 0000	80	50	P
//	101 0001	81	51	Q
//	101 0010	82	52	R
//	101 0011	83	53	S
//	101 0100	84	54	T
//	101 0101	85	55	U
//	101 0110	86	56	V
//	101 0111	87	57	W
//	101 1000	88	58	X
//	101 1001	89	59	Y
//	101 1010	90	5A	Z
//	101 1011	91	5B	[
//	101 1100	92	5C	\
//	101 1101	93	5D	]
//	101 1110	94	5E	^
//	101 1111	95	5F	_
//	110 0000	96	60	`
//	110 0001	97	61	a
//	110 0010	98	62	b
//	110 0011	99	63	c
//	110 0100	100	64	d
//	110 0101	101	65	e
//	110 0110	102	66	f
//	110 0111	103	67	g
//	110 1000	104	68	h
//	110 1001	105	69	i
//	110 1010	106	6A	j
//	110 1011	107	6B	k
//	110 1100	108	6C	l
//	110 1101	109	6D	m
//	110 1110	110	6E	n
//	110 1111	111	6F	o
//	111 0000	112	70	p
//	111 0001	113	71	q
//	111 0010	114	72	r
//	111 0011	115	73	s
//	111 0100	116	74	t
//	111 0101	117	75	u
//	111 0110	118	76	v
//	111 0111	119	77	w
//	111 1000	120	78	x
//	111 1001	121	79	y
//	111 1010	122	7A	z
//	111 1011	123	7B	{
//	111 1100	124	7C	|
//	111 1101	125	7D	}
//	111 1110	126	7E	~
//
