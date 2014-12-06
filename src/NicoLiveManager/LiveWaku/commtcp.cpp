#include "commtcp.h"
#include "../../mainwindow.h"
#include "nowlivewaku.h"

CommTcp::CommTcp(QString domain, int port, QString thread, MainWindow* mwin, NowLiveWaku* nlwaku) :
  socket(NULL)
{
  this->domain = domain;
  this->port = port;
  this->thread = thread;
  this->mwin = mwin;
  this->nlwaku = nlwaku;

  socket = new QTcpSocket(this);

  connect(socket, SIGNAL(connected()),this, SLOT(connected()));
  connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
  connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}

void CommTcp::doConnect()
{
  socket->connectToHost(domain, port);

  if(!socket->waitForConnected(5000)) {
    throw QString("CommTcp::doConnect Error: ").append(socket->errorString());
  }
}

void CommTcp::connected()
{
  QByteArray send;

  send.append("<thread thread=\""+thread+"\" res_from=\"-1000\" version=\"20061206\" />");
  send.append('\0');

  if (socket->write(send) == -1) {
    throw QString("CommTcp::connected Error: ").append(socket->errorString());
  }

  // set timer to send NULL data.
  nullData_timer.setInterval(60000);
  connect(&nullData_timer, SIGNAL(timeout()), this, SLOT(sendNull()));
  nullData_timer.start();
}

void CommTcp::disconnected()
{
  mwin->onReceiveEnded();
}

void CommTcp::sendNull()
{
  QByteArray send(1, '\0');

  if (socket->write(send) == -1) {
    mwin->insLog("CommTcp::sendNull Error: " + socket->errorString() + "\n");
    mwin->bodyClear();
    socket->close();
    doConnect();
  }
}

void CommTcp::readyRead()
{
  QList<QByteArray> rawcomms( socket->readAll().split('\0') );
  rawcomms[0].insert(0, lastRawComm);

  for ( int i = 0; i < rawcomms.size()-1; ++i) {
    readOneRawComment(rawcomms[i]);
  }

  lastRawComm = rawcomms.takeLast();
}

void CommTcp::readOneRawComment(QByteArray& rawcomm)
{
  if (rawcomm.startsWith("<thread")) {
    open_time = QDateTime::currentDateTime();
    mwin->onReceiveStarted();

    StrAbstractor threadinfo(rawcomm);
    lastBlockNum = threadinfo.midStr("last_res=\"", "\"", false).toInt() / 10;
    ticket = threadinfo.midStr("ticket=\"", "\"", false);
    server_time = threadinfo.midStr("server_time=\"", "\"", false).toUInt();

    nlwaku->getPostKeyAPI(thread, lastBlockNum);
    // set timer to get post_key
    postkey_timer.setInterval(10000);
    connect(&postkey_timer, SIGNAL(timeout()), nlwaku, SLOT(getPostKeyAPI()));
    postkey_timer.start();

    return;
  }
  if (rawcomm.startsWith("<chat_result")) {
    StrAbstractor commresult(rawcomm);
    const auto status = commresult.midStr("status=\"", "\"", false);
    if (status != "0") {
      mwin->insLog("CommTcp::readOneRawComment sendComment error with statue " + status);
    }
    mwin->submittedComment();
    return;
  }

  int commSt = rawcomm.indexOf(">") + 1;
  int commEd = rawcomm.lastIndexOf("</chat>");

  QString comm = QString(rawcomm.mid(commSt,commEd-commSt));
  rawcomm.truncate(commSt);

  StrAbstractor comminfo(rawcomm);

  int num = comminfo.midStr("no=\"", "\"", false).toInt();
  const int block = num/10;
  if (block > lastBlockNum) {
    lastBlockNum = block;
    nlwaku->getPostKeyAPI(thread, block);
  }

  int udate = comminfo.midStr("date=\"", "\"", false).toInt();
  QDateTime commenttime;
  commenttime.setTime_t(udate);
  QString date = commenttime.toString("yyyy/MM/dd hh:mm:ss");

  QString mail = comminfo.midStr("mail=\"", "\"", false);
  bool is_184 = (mail.indexOf("184")!=-1) ? true : false;

  QString user = comminfo.midStr("user_id=\"", "\"", false);

  bool premium = false, broadcaster = false;
  QString premS = comminfo.midStr("premium=\"", "\"", false);
  if ( !premS.isEmpty() ) {
    int prem = premS.toInt();
    if (prem % 2) premium = true;
    if ((prem >> 1) % 2 ) broadcaster = true;
  }

  if (comm == "/disconnect" && broadcaster) {
    mwin->on_disconnect_clicked();
  }

  // html code decode
  comm.replace("&amp;", "&");
  comm.replace("&lt;", "<");
  comm.replace("&gt;", ">");

  mwin->insComment( num, premium,
                    broadcaster?"放送主":user, comm, date,
                    is_184, broadcaster, commenttime > open_time);

  // comment command
  if ( mwin->isCommandCommentChecked() && commenttime > open_time ) {
    QProcess pr;
    QString cmd = mwin->getCommandComment();

    QString escmd = comm;
    escmd.replace("'", "");
    cmd.replace("%comment%",'"' + escmd + '"');

    pr.start(cmd);
    pr.waitForFinished(30000);
  }
}

void CommTcp::close()
{
  nullData_timer.stop();
  socket->close();
}

void CommTcp::sendComment(const QString& text)
{
  const auto start_time = nlwaku->getSt().toTime_t();
  const auto now_time = QDateTime::currentDateTime();

  QString postkey = nlwaku->getPostKey();
  if (postkey == "") {
    mwin->insLog("CommTcp::sendComment no postKey\n");
    return;
  }

  QByteArray send;
  open_time = QDateTime::currentDateTime();

  const auto vpos = 100 * (server_time - start_time +
                     now_time.toTime_t() - open_time.toTime_t());

  send.append("<chat thread=\"" + thread +
              "\" ticket=\"" + ticket +
              "\" vpos=\"" + QString::number(vpos) +
              "\" postkey=\"" + postkey +
              // "\" mail=\" 184\"" +
              "\" user_id=\"" + nlwaku->getUser_id() +
              QString("\"") +
              ((nlwaku->getIs_premium())?" premium=\"1\"":"") +
              QString(">") + text +
              "</chat>");
  send.append('\0');

  if (socket->write(send) == -1) {
    throw QString("CommTcp::sendComment Error: ").append(socket->errorString());
  }
}

bool CommTcp::isConnected()
{
  if (socket == NULL) return false;
  return socket->state() != QAbstractSocket::UnconnectedState;
}
