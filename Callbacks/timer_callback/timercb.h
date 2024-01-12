typedef void (*callback_t)(void);

// Timer callback context which holds pointer to registred callback funcition & other user data
typedef struct timercb{
    callback_t callbackHandler;
    int sleeptime;
}timercb_t;

void register_callback(timercb_t *timerCtx);