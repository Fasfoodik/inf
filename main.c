#include <stdio.h>
#define MEM_SIZE 256
#define CPU_SIZE 8
int main(void){
    int memory[MEM_SIZE]={0};
    int cpu_state[CPU_SIZE] = {0};
     /*
     * cpu_state[0] - счетчик команд (pc)
     * cpu_state[1] - аккумулятор (acc), хранит текущий результат
     * cpu_state[2] - регистр команд (ir), хранит текущий опкод
     * cpu_state[3] - вектор прерываний (адрес обработчика ошибок)
     * cpu_state[4] - флаг остановки (0 = работает, 1 = остановлен)
     * cpu_state[5] - код последней ошибки (0 = нет ошибки)
     * cpu_state[6] - резерв для будущих расширений
     * cpu_state[7] - резерв для будущих расширений
     */
    FILE *f = fopen("program.txt", "r");
if (f == NULL)
    return -1;

    int i = 0;
while (i < MEM_SIZE && fscanf(f, "%d", &memory[i]) == 1)
    i++;
fclose(f);
cpu_state[0] = 3;
cpu_state[3] = memory[0]; 
int addr = 0;             


printf("%d %d %d %d\n", memory[0], memory[1], memory[2], memory[3]);
    return 0;
}