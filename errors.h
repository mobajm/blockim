#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <SDL/SDL.h>                    /* SDL_GetError */
#include <string.h>                     /* strerror */
#include <stdio.h>                      /* fprintf */
#include <errno.h>                      /* errno */

#define SDL_ERR(s) fprintf(stderr, "%s: %s.\n", s, SDL_GetError())
#define ERR(s) fprintf(stderr, "%s: %s.\n", s, strerror(errno))
#define INFO(s) fprintf(stdout, "%s.\n", s)

#endif /* _ERRORS_H_ */
