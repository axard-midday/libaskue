#ifndef COMMAND_LINE_INTERFACE_H_
#define COMMAND_LINE_INTERFACE_H_

typedef int ( *cli_handler_f )( void *ptr, const char *arg );

typedef enum _cli_arg_type_t
{
    CLI_SHORT_ARG,
    CLI_LONG_ARG,
    CLI_UNKNOW_ARG
} cli_arg_type_t;

typedef enum _cli_arg_need_t
{
    CLI_REQUIRED_ARG,
    CLI_OPTIONAL_ARG,
    CLI_NO_ARG
} cli_arg_need_t;

typedef enum _cli_val_need_t
{
    CLI_REQUIRED_VAL,
    CLI_OPTIONAL_VAL,
    CLI_NO_VAL
} cli_val_need_t;

typedef struct _cli_arg_t
{
    const char *LongName;
    int ShortName;
    cli_arg_need_t ArgNeed;
    cli_val_need_t ValNeed;
    cli_handler_f Handler;
    void *Var;
} cli_arg_t;

#define CLI_LAST_ARG ((cli_arg_t){ NULL, 0, CLI_NO_ARG, CLI_NO_VAL, NULL, NULL })

typedef enum _cli_result_t
{
    CLI_SUCCESS = 0,
    CLI_ERROR_NOVAL, // нет требуемого за аргументом значения
    CLI_ERROR_HANDLER, // ошибка обработчика
    CLI_ERROR_NEEDARG, // ошибка с отсутствием в аргументах программы обязательных аргументов
    CLI_ERROR_ARGTYPE // ошибка определения аргумента ( он не начинается с символа '-' или '-' )
    
} cli_result_t;

cli_result_t cli_parse ( cli_arg_t *Argumet, int argc, char** argv );

#endif /* COMMAND_LINE_INTERFACE_H_ */
