unsigned short PrevError = 0, IntTerm = 0;
unsigned short PrevError_C = 0, IntTerm_C = 0;

/**
  * @brief  PID in C, Error computed outside the routine
  * @param ror: difference between reference and measured value
  *   Coeff: pointer to the coefficient table
  * @retval : PID output (command)
  */
unsigned short DoPID(unsigned short Error, unsigned short *Coeff)
{
  unsigned short Kp, Ki, Kd, Output;

  Kp = Coeff[0];
  Ki = Coeff[1];
  Kd = Coeff[2];

  IntTerm_C += Ki*Error;
  Output = Kp * Error;
  Output += IntTerm_C;
  Output += Kd * (Error - PrevError_C);

  PrevError_C = Error;

  return (Output);
}



/**
  * @brief  PID in C, Error computed inside the routine
  * @param : Input (measured value)
  *   Ref: reference (target value)
  *   Coeff: pointer to the coefficient table
  * @retval : PID output (command)
  */
unsigned short DoFullPID(unsigned short In, unsigned short Ref, unsigned short *Coeff)
{
  unsigned short Kp, Ki, Kd, Output, Error;

  Error = Ref - In;
  Kp = Coeff[0];
  Ki = Coeff[1];
  Kd = Coeff[2];

  IntTerm_C += Ki*Error;
  Output = Kp * Error;
  Output += IntTerm_C;
  Output += Kd * (Error - PrevError_C);

  PrevError_C = Error;

  return (Output);
}