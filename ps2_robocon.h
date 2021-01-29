#define PS2_INIT 1
#include <avr/io.h>
#include <stdbool.h>

#define SET(x,b) x|=(1<<b)
#define CLR(x,b) x=x&(~(1<<b))
#define CHK(x,b) (x&(1<<b))
#define TOG(x,b) x^=(1<<b)
#define _BIT_(x) (1<<x)
#define _BIT_CLR(x,a) (x &= (~(a)))
#define _BIT_SET(x,a) (x |= a)
#define DDR_SPI DDRB
#define S PORTB
#define SPI_MOSI 2
#define SPI_SCK 1
#define SPI_MISO 3
#define SPI_SS 0
#define PS2_INIT 1
void set_ps2()
{
  DDR_SPI |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
  CLR(S, SPI_SS);
  SET(S, SPI_MISO);
  SPCR |= (1 << MSTR) | (1 << CPHA) | (1 << SPE) | (1 << SPR1) | (1 << DORD) | (1 << CPOL);
}

uint8_t spi_master_trxn(char cmd)
{
  SPDR = cmd;
  while (!(SPSR & (1 << SPIF)));
  _delay_us(100);
  return SPDR;
}

#define ps2_remote_att_hi SET(PORTB,0)
#define ps2_remote_att_lw CLR(PORTB,0)

typedef struct
{

  bool up;
  bool down;
  bool left;
  bool right;
  bool L1;
  bool L2;
  bool L3;
  bool R1;
  bool R2;
  bool R3;
  bool square;
  bool circle;
  bool cross;
  bool tri;
  bool start;
  bool select;


  uint8_t ljoy_left_right;
  uint8_t ljoy_up_down;
  uint8_t rjoy_left_right;
  uint8_t rjoy_up_down;

} DATA_PS2;

DATA_PS2 ps2 ;
uint8_t ps2_trxn(uint8_t cmd1);

unsigned char d1, d2, d3;
float tx_ch;
void ps2_data()
{

  unsigned char d[6];

  ps2_remote_att_lw;
  _delay_us(100);
  d1 = ps2_trxn(0x01);
  d2 = ps2_trxn(0x42);
  d3 = ps2_trxn(0x00);
  d[0] = ps2_trxn(0x00);
  d[1] = ps2_trxn(0x00);
  d[2] = ps2_trxn(0x00);
  d[3] = ps2_trxn(0x00);
  d[4] = ps2_trxn(0x00);
  d[5] = ps2_trxn(0x00);
  ps2_remote_att_hi;

  //digital data

  ps2.up    = (bool)(!(d[0] & 0x10));
  ps2.right = (bool)(!(d[0] & 0x20));
  ps2.down  = (bool)(!(d[0] & 0x40));
  ps2.left  = (bool)(!(d[0] & 0x80));
  ps2.select  = (bool)(!(d[0] & 0x01));
  ps2.L3    = (bool)(!(d[0] & 0x02));
  ps2.R3    = (bool)(!(d[0] & 0x04));
  ps2.start = (bool)(!(d[0] & 0x08));
  ps2.L2    = (bool)(!(d[1] & 0x01));
  ps2.R2    = (bool)(!(d[1] & 0x02));
  ps2.L1    = (bool)(!(d[1] & 0x04));
  ps2.R1    = (bool)(!(d[1] & 0x08));
  ps2.tri   = (bool)(!(d[1] & 0x10));
  ps2.circle  = (bool)(!(d[1] & 0x20));
  ps2.cross  = (bool)(!(d[1] & 0x40));
  ps2.square = (bool)(!(d[1] & 0x80));

  //analog data
  if (d2 == 0x73)
  {
    ps2.ljoy_left_right = d[4];
    ps2.ljoy_up_down = d[5];
    ps2.rjoy_left_right = d[2];
    ps2.rjoy_up_down = d[3];
  }
  else
  {
    ps2.ljoy_left_right = 128;
    ps2.ljoy_up_down = 128;
    ps2.rjoy_left_right = 128;
    ps2.rjoy_up_down = 128;
  }
}
uint8_t ps2_trxn(uint8_t cmd1)
{
  uint8_t temp;
  temp = spi_master_trxn(cmd1);
  return temp;
}
