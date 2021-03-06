/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _EX7_H_RPCGEN
#define _EX7_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define REMOTE_PROG 17
#define MESSAGEVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define SMALL_ALPHA 1
extern  int * small_alpha_1(char **, CLIENT *);
extern  int * small_alpha_1_svc(char **, struct svc_req *);
#define SUM 2
extern  double * sum_1(double *, CLIENT *);
extern  double * sum_1_svc(double *, struct svc_req *);
#define THE_SAME 3
extern  int * the_same_1(char **, CLIENT *);
extern  int * the_same_1_svc(char **, struct svc_req *);
extern int remote_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define SMALL_ALPHA 1
extern  int * small_alpha_1();
extern  int * small_alpha_1_svc();
#define SUM 2
extern  double * sum_1();
extern  double * sum_1_svc();
#define THE_SAME 3
extern  int * the_same_1();
extern  int * the_same_1_svc();
extern int remote_prog_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_EX7_H_RPCGEN */
