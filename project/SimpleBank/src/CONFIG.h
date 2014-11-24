
#ifndef SimpleBank_CONFIG_h
#define SimpleBank_CONFIG_h

/*CONFIGURATIONS*/
#define DEFAULT_MANAGER "3307"
#define DEFAULT_MAINTENANCE "mgr"
#define DEFAULT_PASSWORD "pass"
#define DEFAULT_CREDIT_LIMIT 2000
#define FAILED_PAYMENT_LOG_NAME "FailedCreditPayments.txt"
#define LOG_FILENAME "trace.txt"

/*SYSTEM CONSTANTS, DO NOT MODIFY*/
#define PAY_MIN_10 0
#define PAY_FULL 1

#define HIDE_TRANSACTIONS_OFF 0
#define HIDE_TRANSACTIONS_ON 1

#define ACCOUNT_TYPE_SAVINGS 0
#define ACCOUNT_TYPE_CHECKING 1
#define ACCOUNT_TYPE_CREDIT 2

#define YES 1
#define NO 0

#endif
