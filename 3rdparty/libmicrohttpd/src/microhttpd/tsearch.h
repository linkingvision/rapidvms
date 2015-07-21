/*-
 * Written by J.T. Conklin <jtc@netbsd.org>
 * Public domain.
 *
 *	$NetBSD: search.h,v 1.12 1999/02/22 10:34:28 christos Exp $
 * $FreeBSD: release/9.0.0/include/search.h 105250 2002-10-16 14:29:23Z robert $
 */

#ifndef _SEARCH_H_
#define _SEARCH_H_

#ifndef _MSC_FULL_VER
#include <sys/cdefs.h>
#endif  /* _MSC_FULL_VER */
#if !defined(__BEGIN_DECLS) || !defined(__END_DECLS)
#if defined(__cplusplus)
#define	__BEGIN_DECLS	extern "C" {
#define	__END_DECLS	};
#else  /* !__cplusplus */
#define	__BEGIN_DECLS
#define	__END_DECLS
#endif /* !__cplusplus */
#endif /* !__BEGIN_DECLS || !__END_DECLS */
#include <sys/types.h>

typedef	enum {
	preorder,
	postorder,
	endorder,
	leaf
} VISIT;

#ifdef _SEARCH_PRIVATE
typedef	struct node {
	char         *key;
	struct node  *llink, *rlink;
} node_t;
#endif

__BEGIN_DECLS
void	*tdelete(const void * __restrict, void ** __restrict,
	    int (*)(const void *, const void *));
void	*tfind(const void *, void * const *,
	    int (*)(const void *, const void *));
void	*tsearch(const void *, void **, int (*)(const void *, const void *));
void	 twalk(const void *, void (*)(const void *, VISIT, int));
void	 tdestroy(void *, void (*)(void *));
__END_DECLS

#endif /* !_SEARCH_H_ */
