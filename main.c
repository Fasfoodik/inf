#include <stdio.h>
#define CPU_STATE_SIZE 8
#define MEMORY_SIZE 256

int init_memory(int *memory);
void print_info(int op_code, int *cpu_state, int operand_addr);

int main(void)
{
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
    int cpu_state[CPU_STATE_SIZE] = {0};

    int memory[MEMORY_SIZE] = {0};

    int status = init_memory(memory);
    if (status != 0)
    {
        printf("Ошибка: не удалось открыть файл program.txt\n");
        return status;
    }

    int operand_addr = 0;

    cpu_state[0] = 3;         // адрес первой команды
    cpu_state[3] = memory[0]; // вектор прерываний

    while (cpu_state[4] == 0)
    {
        if (cpu_state[0] < 0 || cpu_state[0] >= MEMORY_SIZE)
        {
            printf("pc = %d, команда = %d -> ОШИБКА: выход за пределы памяти\n", cpu_state[0], cpu_state[2]);
            cpu_state[5] = 1;            // код ошибки
            cpu_state[0] = cpu_state[3]; // переход на вектор прерываний
            continue;
        }

        cpu_state[2] = memory[cpu_state[0]]; // текущая команда

        if (cpu_state[2] != 0)
        {
            if (cpu_state[0] + 1 >= MEMORY_SIZE)
            {
                printf("pc = %d, команда = %d -> ОШИБКА: выход за пределы памяти\n", cpu_state[0], cpu_state[2]);
                cpu_state[5] = 1;
                cpu_state[0] = cpu_state[3];
                continue;
            }
            operand_addr = memory[cpu_state[0] + 1]; // адрес операнда
        }

        if (cpu_state[2] != 0 && (operand_addr < 0 || operand_addr >= MEMORY_SIZE))
        {
            printf("pc = %d, команда = %d -> ОШИБКА: выход за пределы памяти\n", cpu_state[0], cpu_state[2]);
            cpu_state[5] = 1;
            cpu_state[0] = cpu_state[3];
            continue;
        }

        switch (cpu_state[2])
        {
        case 1: // LOAD
            cpu_state[1] = memory[operand_addr];
            print_info(1, cpu_state, operand_addr);
            cpu_state[0] += 2;
            break;

        case 2: // STORE
            memory[operand_addr] = cpu_state[1];
            print_info(2, cpu_state, operand_addr);
            cpu_state[0] += 2;
            break;

        case 3: // ADD
            cpu_state[1] += memory[operand_addr];
            print_info(3, cpu_state, operand_addr);
            cpu_state[0] += 2;
            break;

        case 4: // JUMP
            print_info(4, cpu_state, operand_addr);
            cpu_state[0] = operand_addr;
            break;

        case 5: // JUMP IF ZERO
            print_info(5, cpu_state, operand_addr);
            if (cpu_state[1] == 0)
                cpu_state[0] = operand_addr;
            else
                cpu_state[0] += 2;
            break;

        case 0: // HALT
            cpu_state[4] = 1;
            print_info(0, cpu_state, 0);
            break;

        default:
            printf("pc = %d, команда = %d -> ОШИБКА: неизвестная команда\n", cpu_state[0], cpu_state[2]);
            cpu_state[5] = 2;
            cpu_state[0] = cpu_state[3];
            break;
        }
    }

    return 0;
}

int init_memory(int *memory)
{
    FILE *f = fopen("program.txt", "r");
    if (f == NULL)
        return -1;

    size_t i = 0;
    while (i < MEMORY_SIZE && fscanf(f, "%d", &memory[i]) == 1)
        i++;

    fclose(f);

    return 0;
}

void print_info(int op_code, int *cpu_state, int operand_addr)
{
    switch (op_code)
    {
    case 1:
        printf("pc = %d: команда %d (LOAD) -> адрес %d -> acc = %d\n", cpu_state[0], cpu_state[2], operand_addr, cpu_state[1]);
        break;

    case 2:
        printf("pc = %d: команда %d (STORE) -> адрес %d -> memory[%d] = %d\n", cpu_state[0], cpu_state[2], operand_addr, operand_addr, cpu_state[1]);
        break;

    case 3:
        printf("pc = %d: команда %d (ADD) -> адрес %d -> acc = %d\n", cpu_state[0], cpu_state[2], operand_addr, cpu_state[1]);
        break;

    case 4:
        printf("pc = %d: команда %d (JUMP) -> адрес %d -> pc = %d\n", cpu_state[0], cpu_state[2], operand_addr, operand_addr);
        break;

    case 5:
        printf("pc = %d: команда %d (JUMP IF ZERO) -> адрес %d -> acc = %d\n", cpu_state[0], cpu_state[2], operand_addr, cpu_state[1]);
        break;

    case 0:
        printf("pc = %d: команда %d (HALT) -> стоп\n", cpu_state[0], cpu_state[2]);
        break;

    default:
        break;
    }
}
