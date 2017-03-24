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
255};

int tablesel[] = {1,2,4,8};

int i = 100;
void setup() {
  DDRB = 0b111111;
  DDRD = 0xff; 
}

void loop() {
  PORTD = table[0];
  PORTB = tablesel[0];
  delay(i);
  PORTD = table[1];
  PORTB = tablesel[1];
  delay(i);
  PORTD = table[2];
  PORTB = tablesel[2];
  delay(i);
  PORTD = table[3];
  PORTB = tablesel[3];
  delay(i);
  PORTD = table[20];
}
