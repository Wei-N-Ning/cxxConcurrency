# Pipelines

## Advantage of Pipelines

in action 2nd P/283

Pipelines are also good when each operation in the sequence is time-consuming;
by dividing the tasks between threads rather than the data, you change the perfor-
mance profile. Suppose you have 20 data items to process on 4 cores, and each data
item requires 4 steps, which take 3 seconds each. If you divide the data between four
threads, then each thread has five items to process. Assuming there’s no other pro-
cessing that might affect the timings, after 12 seconds you’ll have 4 items processed,
after 24 seconds 8 items processed, and so on. All 20 items will be done after 1 minute.
With a pipeline, things work differently. Each of your four steps can be assigned to a
processing core. Now the first item has to be processed by each core, so it still takes
the full 12 seconds. Indeed, after 12 seconds you only have 1 item processed, which
isn’t as good as with the division by data. But once the pipeline is primed, things pro-
ceed a bit differently; after the first core has processed the first item, it moves on to
the second, so once the final core has processed the first item, it can perform its step
on the second. You now get 1 item processed every 3 seconds rather than having the
items processed in batches of 4 every 12 seconds.

high def video decoding:

Consider, for example, a
system for watching high-definition digital videos. In order for the video to be watch-
able, you typically need at least 25 frames per second and ideally more. Also, the
viewer needs these to be evenly spaced to give the impression of continuous move-
ment; an application that can decode 100 frames per second is still of no use if it
pauses for a second, then displays 100 frames, then pauses for another second, and
displays another 100 frames. On the other hand, viewers are probably happy to accept
a delay of a couple of seconds when they start watching a video. 

