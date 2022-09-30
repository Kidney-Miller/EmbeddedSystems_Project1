void dly(unsigned int dlx);
char get_keypd_key(void);

void Keypad_Pres(void)
{
    char K_Val = 0;
    while(1)
    {
        K_Val = get_keypd_key();
        dely(20);
    }
}

char get_keypd_key(void)
{
    char kay_val = 0;
    GPIOB->ODR &= 0x00FE; //Change for GPIOB = ROW 1
    if(GBIOB->ODR == 0x00EE){
        key_val = '1';
    }
    if(GPIOB->ODR == 0x00DE){
        key_val = '2';
    }
    if(GPIOB->ODR == 0x00BE){
        key_val = '3';
    }
    GPIOB->ODR = 0x00FD; //Change fpr GPIOB = ROW 2
    if(GPIOB->ODR == 0x00ED){ //Column 1
        key_val = '4';
    }
    if(GPIOB->ODR == 0x00DD){ //Column 2
        key_val = '5';
    }
    if(GPIOB->ODR == 0x00BD){ //Column 3
        key_val = '6';
    }
    GPIOB->ODR = 0x00FB; //Change for GPIOB = ROW 3
    if(GPIOB->ODR == 0x00EB){ //Column 1
        key_val = '7';
    }
    if(GPIOB->ODR == 0x00DB){ // Column 2
        key_val = '8';
    }
    if(GPIOB->ODR == 0x00BB){ // Column 3
        key_val = '9';
    }
    GPIOB->ODR = 0x00F7; //Change for GPIOB = ROW 4
    if(GPIOB->ODR == 0x00E7) // Column 1
    {
        key_val = '*';
    }
    if(GPIOB->ODR == 0x00D7) // Column 2
    {
        key_val = '0';
    }
    if(GPIOB->ODR == 0x00B7) // Column 3
    {
        key_val = '#';
    }
    return key_val;
}
