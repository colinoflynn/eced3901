/*

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#include "twimaster.h"
#include "mpu9150.h"

static int uart_putchar(char c, FILE *stream);
static int uart_getchar(FILE *stream);
FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE mystdin = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

static int uart_getchar(FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return UDR0;
}

void init_uart(void)
{
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UBRR0 = 7;
	stdout = &mystdout;
	stdin = &mystdin;
}

int main(void)
{
	init_uart();
	printf_P(PSTR("System Booted, built %s on %s\n"), __TIME__, __DATE__);
	
	i2c_init();
	
	printf_P(PSTR("MPU9150: Attempting Init Call.. if system hangs here check I2C connections\n"));	
	Initalise_AccelGyro(ACCELRANGE_2g, GYRORANGE_250DPS);
	printf_P(PSTR("MPU9150: Init Done\n"));
	
	int xacc, yacc, zacc;
	int xgyr, ygyr, zgyr;
	int xmag, ymag, zmag;
	
	while(1){		
		
		Trigger_Compass();
		
		xacc = Read_Acc_Gyro(ACCEL_XOUT_H);
		yacc = Read_Acc_Gyro(ACCEL_YOUT_H);
		zacc = Read_Acc_Gyro(ACCEL_ZOUT_H);
		
		xgyr = Read_Acc_Gyro(GYRO_XOUT_H);
		ygyr = Read_Acc_Gyro(GYRO_YOUT_H);
		zgyr = Read_Acc_Gyro(GYRO_ZOUT_H);
					
		xmag = Read_Compass(COMP_XOUT_L);
		ymag = Read_Compass(COMP_YOUT_L);
		zmag = Read_Compass(COMP_ZOUT_L);

		_delay_ms(200);

		printf("%+6d %+6d %+6d # %+6d %+6d %+6d # %+6d %+6d %+6d\n", xacc, yacc, zacc, xgyr, ygyr, zgyr, xmag, ymag, zmag);			
	}
}