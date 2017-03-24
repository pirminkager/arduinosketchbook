int table[] = {192,
249,
164,
176,
153,
146,
130,
248,
128,
144,
64,
121,
36,
48,
25,
18,
2,
120,
0,
16,
255,
0b11111110,
0b11111101,
0b11111011,
0b11110111,
0b11101111,
0b11011111,
0b10111111,
0b01111111,
0b11111111};

int tablesel[] = {1,2,4,8};

int i = 1000;
int d = 0;
int c = 0;
void setup() {
  DDRB = 0b111111;
  DDRD = 0xff; 
}

void loop() {
  
  PORTD = table[20];
  PORTB = tablesel[0];
  PORTD = table[d];
  delay(i);
  PORTD = table[20];
  PORTB = tablesel[1];
  PORTD = table[d+1];
  delay(i);
  PORTD = table[20];
  PORTB = tablesel[2];
  PORTD = table[d+2];
  delay(i);
  PORTD = table[20];
  PORTB = tablesel[3];
  PORTD = table[d+3];
  delay(i);
  PORTD = table[20];

  if (c >= 5) {
    d = d+1;
    c = 0;
  }
  c = c+1;
  
  if (d >= 26) {
    d = 0;
  }
}
