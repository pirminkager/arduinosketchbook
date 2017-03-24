int table[] = {192,
249,
164,
176,
153,
146,
130,
248,
128,
151,
64,
121,
36,
48,
25,
18,
2,
120,
0,
23,
255};

int tablesel[] = {1,2,4,8};

int seldig1 = 8;
int seldig2 = 9;
int seldig3 = 10;
int seldig4 = 11;

void setup() {
  DDRB = 0b111111;
  DDRD = 0xff; 
}

void loop() {
  
  PORTD = table[20];
  PORTB = tablesel[0];
  PORTD = table[0];
  PORTD = table[20];
  PORTB = tablesel[1];
  PORTD = table[1];
  PORTD = table[20];
  PORTB = tablesel[2];
  PORTD = table[2];
  PORTD = table[20];
  PORTB = tablesel[3];
  PORTD = table[3];
}
