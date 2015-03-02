#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include "mac2.h"
#include "bstrlib.h"
#include "dbg.h"

#define GPIO_TX_PIN
#define GPIO_RX_PIN

FILE *tx = NULL;
FILE *rx = NULL;

/* Description: Init the gpio pins used for the demo.
 * Author: Albin Severinson
 * Date: 28/02/15
 */
int init_gpio()
{
  FILE *export = NULL;
  FILE *gpio_tx_dir = NULL;
  FILE *gpio_rx_dir = NULL;
  /*
  //Export GPIO pins
  export = fopen("/sys/class/gpio/export", "w");
  check(export, "Failed to export GPIO.");

  char c = 60;
  fprintf(export, "%d", c);
  //fprintf(export, "60\n");
  fprintf(export, "%d", 50);

  fflush(export);
  fclose(export);

  //Set the direction
  gpio_tx_dir = fopen("/sys/class/gpio/gpio60/direction", "w");
  check(gpio_tx_dir, "Failed to set GPIO direction for tx.");

  gpio_rx_dir = fopen("/sys/class/gpio/gpio50/direction", "w");
  check(gpio_rx_dir, "Failed to set GPIO direction for rx.");

  fprintf(gpio_tx_dir, "high");
  fprintf(gpio_rx_dir, "in");

  fflush(gpio_tx_dir);
  fflush(gpio_rx_dir);
  fclose(gpio_tx_dir);
  fclose(gpio_rx_dir);
  */

  //Open the TX and RX pins
  tx = fopen("/sys/class/gpio/gpio60/value", "w");
  check(tx, "Failed to open tx GPIO.");

  //rx = fopen("/sys/class/gpio/gpio50/value", "r");
  //check(rx, "Failed to open rx GPIO.");
  
  return 0;

 error:
  if(export) fclose(export);
  if(gpio_tx_dir) fclose(gpio_tx_dir);
  if(gpio_rx_dir) fclose(gpio_rx_dir);
  if(tx) fclose(tx);

  return -1;
}

/* Description: Recieve a bit from the MAC2 layer and send over the
   physical layer.
 * Author:
 * Date:
 */
int send_bit(char bit)
{
  assert(tx && "GPIO not initialized.");
  fprintf(tx, "%d", bit);
  fflush(tx);
  usleep(100000);
  return 0;
}

//For DEMO
int get_analog_bit()
{
  FILE *ain1 = NULL;
  ain1 = fopen("/sys/devices/ocp.3/helper.15/AIN1", "r");
  check(ain1, "Failed tp open AIN1.");

  int int_value = 0;
  char value[5] = {0};
  int len = fread(&value, sizeof(char), 5, ain1);
  fclose(ain1);
  
  int_value = atoi(value);

  debug("Value: %d", int_value);
  return value;

 error:
  if(ain1) fclose(ain1);
  return -1;

}

/* Description: Get a bit from the physical layer and send to the MAC2
   layer.
 * Author:
 * Date:
 */
char get_bit()
{
  //assert(rx && "GPIO not initialized.");
  rx = fopen("/sys/class/gpio/gpio50/value", "r");
  check(rx, "Failed to open rx GPIO.");

  char c = fgetc(rx);

  fflush(rx);
  fclose(rx);

  int rc = get_analog_bit();
  //debug("Analog read: %d", rc);

  if(c == 48) return 0;
  else return 1;

 error:
  if(rx) fclose(rx);
  return -1;
}
