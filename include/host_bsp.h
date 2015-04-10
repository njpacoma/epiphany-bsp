/*
File: bsp_host.h

This file is part of the Epiphany BSP library.

Copyright (C) 2014 Buurlage Wits
Support e-mail: <info@buurlagewits.nl>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License (LGPL)
as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
and the GNU Lesser General Public License along with this program,
see the files COPYING and COPYING.LESSER. If not, see
<http://www.gnu.org/licenses/>.
 */

#pragma once
#include <e-hal.h>

/** This writes data from the host processor to the co-processor.
 *  This can be useful for distributing initial data, or when dividing work
 *  betewen the host and the Epiphany.
 *
 *  pid: The processor ID in the BSP system.
 *  src: A pointer to the source of the data to write.
 *  dst: The destination on the chip processor. (e.g. 0x2000)
 *  size: Amount of data to write in bytes.
 *  flag: 1 on success, 0 on failure
 */
int ebsp_write(int pid, void* src, off_t dst, int size);

/** This reads data from the co-processor to the host processor.
 *  This can be useful for distributing initial data, or when dividing work
 *
 *  pid: The processor ID in the BSP system.
 *  src: The source of the data on the co-processor (e.g. 0x2000)
 *  dst: A destination pointer on the host processor.
 *  size: Amount of data to read in bytes.
 *  flag: 1 on success, 0 on failure
 */
int ebsp_read(int pid, off_t src, void* dst, int size);

/** Initializes the BSP system. This sets up all the BSP variables and loads
 *  the epiphany BSP program.
 *
 *  e_name: A string containing the name of the eBSP program.
 *  argc: An integer containing the number of input arguments
 *  argv: An array of strings containg the input flags.
 *  flag: An integer indicating whether the function finished
 *                succesfully, in which case it is 1, or 0 otherwise.
 */
int bsp_init(const char* e_name,
		int argc,
		char **argv);


/** Set the callback for syncing
 */
void ebsp_set_sync_callback(void (*cb)());

/** Set the callback for finalizing
 */
void ebsp_set_end_callback(void (*cb)());

/** Starts the SPMD program on the Epiphany cores.
 *  flag: An integer indicating whether the function finished
 *                succesfully, in which case it is 1, or 0 otherwise.
 */
int ebsp_spmd();

/** Starts the BSP program.
 *
 *  nprocs: An integer indicating the number of processors to run on.
 *  flag: An integer indicating whether the function finished
 *                succesfully, in which case it is 1, or 0 otherwise.
 */
int bsp_begin(int nprocs);

/** Finalizes and cleans up the BSP program.
 *  flag: An integer indicating whether the function finished
 *                succesfully, in which case it is 1, or 0 otherwise.
 */
int bsp_end();

/** Returns the number of available processors.
 *
 *  nprocs: An integer indicating the number of available processors.
 */
int bsp_nprocs();

/* BSP Message Passing
 *
 * These functions can be used to send messages to message queue
 * of the programs for initialization and retrieve messages to gather results.
 * The initialization messages will only remain in the queue until bsp_sync
 * has been called for the first time.
 * The default tag-size is zero.
 *
 * Sending messages must be done after bsp_init
 * Retrieving messages must be done before bsp_end
 *
 * See e_bsp.h for more information
 */

/* Set initial tagsize.
 * Should be called at most once, before any messages are sent.
 * Calling this when receiving messages results in undefined behaviour.
 */
void ebsp_set_tagsize(int *tag_bytes);

/* Send initial messages */
void ebsp_send_down(int pid, const void *tag, const void *payload, int nbytes);

/* The following functions are only for gathering result messages
 * at the end of a BSP program */

/* Get the tag-size as set by the epiphany cores */
int ebsp_get_tagsize();

/* Get the amount of messages in the queue and their total size */
void ebsp_qsize(int *packets, int *accum_bytes);

/* Peek the next message.
 * Upon return, status holds the amount of bytes of the next message payload,
 * or -1 if there are no more messages.
 * tag will hold the tag of the next message. The buffer pointed to by tag
 * should be large enough (ebsp_get_tagsize).
 */
void ebsp_get_tag(int *status, void *tag);

/* Get the next message and pop it from the queue.
 * Upon return, payload will hold the contents of the message.
 * The buffer will only be filled till at most buffer_size. Remaining
 * data is truncated. Use ebsp_get_tag to get the size of the data payload.
 */
void ebsp_move(void *payload, int buffer_size);

/* Get the pointer to the next message payload, and pop the message.
 * This will be a pointer to external memory so it is slow.
 */
int ebsp_hpmove(void **tag_ptr_buf, void **payload_ptr_buf);

