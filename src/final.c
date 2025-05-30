/* final.c
 */
/* This software is copyrighted as detailed in the LICENSE file. */


#include "EXTERN.h"
#include "common.h"
#include "list.h"
#include "env.h"
#include "util.h"
#include "hash.h"
#include "cache.h"
#include "bits.h"
#include "term.h"
#include "color.h"
#include "ng.h"
#include "init.h"
#include "last.h"
#include "ngdata.h"
#include "nntpclient.h"
#include "nntpinit.h"
#include "datasrc.h"
#include "nntp.h"
#include "rcstuff.h"
#include "artio.h"
#include "intrp.h"
#include "kfile.h"
#ifdef SCORE
#include "score.h"
#include "scoresave.h"
#endif
#ifdef USE_TCL
#include "tkstuff.h"
#endif
#ifdef USE_FILTER
#include "filter.h"
#endif
#include "INTERN.h"
#include "final.h"

#ifndef sigmask
#define sigmask(m)	(1 << ((m)-1))
#endif

void
final_init(void)
{
#ifdef SIGTSTP
    sigset(SIGTSTP, stop_catcher);	/* job control signals */
    sigset(SIGTTOU, stop_catcher);	/* job control signals */
    sigset(SIGTTIN, stop_catcher);	/* job control signals */
#endif

    sigset(SIGINT, int_catcher);	/* always catch interrupts */
#ifdef SIGHUP
    sigset(SIGHUP, sig_catcher);	/* and hangups */
#endif
#ifdef SIGWINCH
    sigset(SIGWINCH, winch_catcher);
#endif
#ifdef SUPPORT_NNTP
    sigset(SIGPIPE,pipe_catcher);
#endif

#ifndef lint
#ifdef SIGEMT
    sigignore(SIGEMT);		/* Ignore EMT signals from old [t]rn's */
#endif
#endif

#ifdef DEBUG
    /* sometimes we WANT a core dump */
    if (debug & DEB_COREDUMPSOK)
	return;
#endif
    sigset(SIGILL, sig_catcher);
#ifdef SIGTRAP
    sigset(SIGTRAP, sig_catcher);
#endif
    sigset(SIGFPE, sig_catcher);
#ifdef SIGBUS
    sigset(SIGBUS, sig_catcher);
#endif
    sigset(SIGSEGV, sig_catcher);
#ifdef SIGSYS
    sigset(SIGSYS, sig_catcher);
#endif
    sigset(SIGTERM, sig_catcher);
#ifdef SIGXCPU
    sigset(SIGXCPU, sig_catcher);
#endif
#ifdef SIGXFSZ
    sigset(SIGXFSZ, sig_catcher);
#endif
}

void					/* very much void */
finalize(int status)
{
#ifdef SUPPORT_NNTP
    int i;
    char* s;
#endif

#ifdef SCORE
    sc_sv_savefile();	/* save any scores from memory to disk */
#endif
#ifdef USE_FILTER
    filter_cleanup();
#endif
#ifdef KILLFILES
    update_thread_kfile();
#endif
    color_default();
    termlib_reset();
    if (bizarre)
	resetty();
    xmouse_off();	/* turn off mouse tracking (if on) */
    fflush(stdout);

    chdir(tmpdir);
    if (!checkflag)
	unuse_multirc(multirc);
    if (datasrc_list) {
	DATASRC* dp;
	for (dp = datasrc_first(); dp && dp->name; dp = datasrc_next(dp))
	    close_datasrc(dp);
    }
#ifdef SUPPORT_NNTP
    for (i = 0; i < MAX_NNTP_ARTICLES; i++) {
	s = nntp_tmpname(i);
	UNLINK(s);
    }
    cleanup_nntp();
#endif
    if (headname)
	UNLINK(headname);
#ifdef USE_TCL
    if (ttcl_running)
	ttcl_finalize(status);
#endif
    if (status < 0) {
	sigset(SIGILL,SIG_DFL);
#ifdef HAS_SIGBLOCK
	/*sigsetmask(sigblock(0) & ~(sigmask(SIGILL) | sigmask(SIGIOT)));*/
        /* This is replacement code for the above line */
#endif
        {
	    sigset_t mask;
	    sigprocmask(SIG_SETMASK, NULL, &mask);
	    sigdelset(&mask, SIGILL);
	    sigdelset(&mask, SIGIOT);
	    sigprocmask(SIG_SETMASK, &mask, NULL);
        }

	abort();
    }
#ifdef RESTORE_ORIGDIR
    if (origdir)
	chdir(origdir);
#endif
    exit(status);
}

/* come here on interrupt */

Signal_t
int_catcher(int dummy)
{
    (void)dummy;

    sigset(SIGINT,int_catcher);
#ifdef DEBUG
    if (debug)
	write(2,"int_catcher\n",12);
#endif
    if (!waiting) {
	if (int_count++) {		/* was there already an interrupt? */
	    if (int_count == 3 || int_count > 5) {
		write(2,"\nBye-bye.\n",10);
		sig_catcher(0);		/* emulate the other signals */
	    }
	    write(2,"\n(Interrupt -- one more to kill trn)\n",37);
	}
    }
}

/* come here on signal other than interrupt, stop, or cont */

Signal_t
sig_catcher(int signo)
{
#ifdef VERBOSE
    static char* signame[] = {
	"",
	"HUP",
	"INT",
	"QUIT",
	"ILL",
	"TRAP",
	"IOT",
	"EMT",
	"FPE",
	"KILL",
	"BUS",
	"SEGV",
	"SYS",
	"PIPE",
	"ALRM",
	"TERM",
	"???"
#ifdef SIGTSTP
	,"STOP",
	"TSTP",
	"CONT",
	"CHLD",
	"TTIN",
	"TTOU",
	"TINT",
	"XCPU",
	"XFSZ"
#ifdef SIGPROF
	,"VTALARM",
	"PROF"
#endif
#endif
	};
#endif

#ifdef DEBUG
    if (debug) {
	printf("\nSIG%s--.newsrc not restored in debug\n",signame[signo]);
	finalize(-1);
    }
#endif
    if (panic) {
#ifdef HAS_SIGBLOCK
	/*sigsetmask(sigblock(0) & ~(sigmask(SIGILL) | sigmask(SIGIOT)));*/
#endif
        /* This is replacement code for the above line */
        {
	    sigset_t mask;
	    sigprocmask(SIG_SETMASK, NULL, &mask);
	    sigdelset(&mask, SIGILL);
	    sigdelset(&mask, SIGIOT);
	    sigprocmask(SIG_SETMASK, &mask, NULL);
        }
	abort();
    }
    (void) sigset(SIGILL,SIG_DFL);
    panic = TRUE;			/* disable terminal I/O */
    if (doing_ng) {			/* need we reconstitute rc line? */
	yankback();
	bits_to_rc();			/* then do so (hope this works) */
    }
    doing_ng = FALSE;
    if (!write_newsrcs(multirc)) {	/* write anything that's changed */
	/*$$ get_old_newsrcs(multirc);  ?? */
    }
#ifdef KILLFILES
    update_thread_kfile();
#endif

#ifdef SIGHUP
    if (signo != SIGHUP) {
#endif
#ifdef VERBOSE
	IF(verbose)
	    printf("\nCaught %s%s--.newsrc restored\n",
		signo ? "a SIG" : "an internal error", signame[signo]);
	ELSE
#endif
#ifdef TERSE
	    printf("\nSignal %d--bye bye\n",signo);
#endif
#ifdef SIGHUP
    }
#endif
    switch (signo) {
#ifdef SIGBUS
    case SIGBUS:
#endif
    case SIGILL:
    case SIGSEGV:
	finalize(-signo);
    }
    finalize(1);				/* and blow up */
}

#ifdef SUPPORT_NNTP
Signal_t
pipe_catcher(int signo)
{
    ;/*$$ we lost the current nntp connection */
    (void)signo;
    sigset(SIGPIPE,pipe_catcher);
}
#endif

/* come here on stop signal */

#ifdef SIGTSTP
Signal_t
stop_catcher(int signo)
{
    if (!waiting) {
	xmouse_off();
	checkpoint_newsrcs();	/* good chance of crash while stopped */
	if (bos_on_stop) {
	    goto_xy(0, tc_LINES-1);
	    putchar('\n') FLUSH;
	}
	termlib_reset();
	resetty();		/* this is the point of all this */
#ifdef DEBUG
	if (debug)
	    write(2,"stop_catcher\n",13);
#endif
	fflush(stdout);
	sigset(signo,SIG_DFL);	/* enable stop */
#ifdef HAS_SIGBLOCK
	/*sigsetmask(sigblock(0) & ~(1 << (signo-1))); */
#endif
        /* This is replacement code for the above line */
        {
	    sigset_t mask;
	    sigprocmask(SIG_SETMASK, NULL, &mask);
	    sigdelset(&mask, signo);
	    sigprocmask(SIG_SETMASK, &mask, NULL);
        }
	kill(0,signo);		/* and do the stop */
    	savetty();
#ifdef MAILCALL
    	mailcount = 0;			/* force recheck */
#endif
    	if (!panic) {
	    if (!waiting) {
		termlib_init();
		noecho();			/* set no echo */
		crmode();			/* set cbreak mode */
		forceme("\f");			/* cause a refresh */
						/* (defined only if TIOCSTI defined) */
		errno = 0;			/* needed for getcmd */
	    }
    	}
	xmouse_check();
    }
    sigset(signo,stop_catcher);	/* unenable the stop */
}
#endif
