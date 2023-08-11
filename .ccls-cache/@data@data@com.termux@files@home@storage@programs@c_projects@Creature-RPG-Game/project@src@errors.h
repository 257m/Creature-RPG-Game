#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <signal.h>

extern void clean_up_program();

#define PANIC(ERROR, FORMAT, ...) {                                              	\
	if (ERROR) {                                                                 	\
		fprintf(stderr, "%s -> %s -> %i -> Error(%i):\n\t" FORMAT, __FILE_NAME__,	\
						__FUNCTION__, __LINE__, ERROR, ##__VA_ARGS__);           	\
		clean_up_program();                                                      	\
		raise(SIGABRT);                                                          	\
	}                                                                            	\
}

#endif /* ERRORS_H */
