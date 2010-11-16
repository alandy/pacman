interrupt 8 void TOC0handler(void);

//---------------------OC_Init0---------------------
// arm output compare 0 for 100Hz periodic interrupt
// Input: none
// Output: none 
void OC_Init0();

unsigned short static volatile Count3;


interrupt 11 void TOC3handler(void);

//---------------------OC_Init3---------------------
// arm output compare 3 for 1000Hz periodic interrupt
// Input: none    assumes another ritual enabled the TCNT
// Output: none 
void OC_Init3();