/**
 * Function stopwatch_open: opens the stopwatch device. 
 * Return: 1 on success, else 0
 */
int	stopwatch_open (void);

/**
 * Function stopwatch_read: reads the stopwatch device
 * Parameter MM: pointer for minutes (00 - 59)
 * Parameter SS: pointer for seconds (00 - 59)
 * Parameter DD: pointer for hundredths (00 - 99)
 * Return: 1 on success, else 0
 */
int	stopwatch_read (int * /*MM*/, int * /*SS*/, int * /*DD*/);

/**
 * Function stopwatch_stop: stops the stopwatch device
 * Return: void
 */
void	stopwatch_stop (void);

/**
 * Function stopwatch_run: runs the stopwatch device
 * Return: void
 */
void	stopwatch_run (void); 

/**
 * Function stopwatch_display: turns on 7-segment display for the stopwatch device
 * Return: void
 */
void	stopwatch_display (void);

/**
 * Function stopwatch_nodisplay: turns off 7-segment display for the stopwatch device
 * Return: void
 */
void	stopwatch_nodisplay (void);

/**
 * Function stopwatch_set: initializes the time for the stopwatch device
 * Parameter MM: minutes (00 - 59)
 * Parameter SS: seconds (00 - 59)
 * Parameter DD: hundredths (00 - 99)
 * Return: void
 */
void stopwatch_set (int /*MM*/, int /*SS*/, int /*DD*/);

/**
 * Function stopwatch_close: closes the stopwatch device
 * Return: void
 */
void	stopwatch_close (void);
