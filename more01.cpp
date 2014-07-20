/* more01.c  - version 0.11 of more
 *	read and print 24 lines then pause for a few special commands
 */

#include <stdio.h>
#include <stdlib.h>

const int PAGE_LEN = 24;
const int LINE_LEN = 512;

void do_more(FILE *);
int see_more();

int main(int argc, char **argv)
{
    if (argc == 1) return do_more(stdin), 0;
    FILE *fp;
    while (--argc)
        if ((fp = fopen(*++argv, "r")) != NULL)
        {
            do_more(fp);
            fclose(fp);
        }
        else exit(EXIT_FAILURE);
    return 0;
}

void do_more(FILE *fp)
/*
 *  read PAGE_LEN lines, then call see_more() for further instructions
 */
{
    char line[LINE_LEN];
    int	num_of_lines = 0;
    int	reply;
    while (fgets(line, LINE_LEN, fp) != NULL) 		/* more input	*/
    {
        if (num_of_lines == PAGE_LEN)  	/* full screen?	*/
        {
            reply = see_more();		/* y: ask user  */
            if (reply == 0)		/*    n: done   */
                break;
            num_of_lines -= reply;		/* reset count	*/
        }
        if (fputs(line, stdout) == EOF)	/* show line	*/
            exit(EXIT_FAILURE);			/* or die	*/
        ++num_of_lines;				/* count it	*/
    }
}

int see_more()
/*
 *	print message, wait for response, return # of lines to advance
 *	q means no, space means yes, CR means one line
 */
{
    char c;
    printf("\033[7m more? \033[m");		/* reverse on a vt100	*/
    while ((c = getchar()) != EOF)			/* get response	*/
    {
        if (c == 'q')			/* q -> N		*/
            return 0;
        if (c == ' ')			/* ' ' => next page	*/
            return PAGE_LEN;		/* how many to show	*/
        if (c == '\n')		/* Enter key => 1 line	*/
            return 1;
    }
    return 0;
}
