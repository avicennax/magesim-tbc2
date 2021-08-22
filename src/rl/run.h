#ifdef __cplusplus
extern "C" {  
#endif  

typedef struct RunResult {
    int dmg;
    double dps;
} RunResult;

RunResult run(int print);

#ifdef __cplusplus  
} // extern "C"  
#endif