

void bt_ready_ISR(void) {

    while (can_receive()) { 

        char data = *(bt_uart);
        strncat(buffer,&data,1);
        if(data == '\n') line_reached = 1;
        data_count++;
    }

    *(hex0) = data_count;

    *(bt_uart + 2 ) = 0;
}