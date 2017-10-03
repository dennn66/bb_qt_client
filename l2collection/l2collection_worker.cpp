#include "l2collection_worker.h"

// --- CONSTRUCTOR ---
L2Collection_Worker::L2Collection_Worker()
{
    // you could copy data from constructor arguments to internal variables here.
    l2collection = new L2Collection();

    connect(this, SIGNAL(turn_completed(bool, int)), l2collection, SLOT(turn_completed(bool, int)), Qt::DirectConnection);

}

L2Collection_Worker::~L2Collection_Worker()
{
    qDebug("L2Collection_Worker::~L2Collection_Worker()");
    delete l2collection;

}

// --- PROCESS ---
// Start processing data.
void L2Collection_Worker::process() {
    //   qDebug("Start l2 checker");
    qDebug("L2Collection_Worker::process()  Thread: %d", (int) QThread::currentThreadId());


    // allocate resources using new here
    while(1){
        QTime updateTime;
        updateTime.start();
        if(l2collection->currentl2w() != NULL) {
            try {
                l2collection->spinonce();
                emit turn_completed(true, updateTime.elapsed());
                qDebug("Check done");
            } catch(...) {
                qDebug("Check failed");
                emit turn_completed(true, updateTime.elapsed());
            }
        }
        int delta = 500 - updateTime.elapsed(); //fix to 50
        delta = (delta > 0)?delta:0;
        #ifdef WIN32
            Sleep(delta);
        #else
            usleep(delta*1000);
        #endif
        qDebug("Loop time: %d", updateTime.elapsed());
    }

    emit finished();
}
