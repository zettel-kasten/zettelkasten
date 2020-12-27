/*
 * Qt4 bitcoin GUI.
 *
 * W.J. van der Laan 2011-2012
 * The Bitcoin Developers 2011-2012
 */

#include <QApplication>

#include "bitcoingui.h"

#include "transactiontablemodel.h"
#include "optionsdialog.h"
#include "aboutdialog.h"
#include "clientmodel.h"
#include "walletmodel.h"
#include "walletframe.h"
#include "optionsmodel.h"
#include "transactiondescdialog.h"
#include "bitcoinunits.h"
#include "guiconstants.h"
#include "notificator.h"
#include "guiutil.h"
#include "rpcconsole.h"
#include "ui_interface.h"
#include "wallet.h"
#include "init.h"

#ifdef Q_OS_MAC
#include "macdockiconhandler.h"
#endif

#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QVBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QStackedWidget>
#include <QDateTime>
#include <QMovie>
#include <QTimer>
#include <QDragEnterEvent>
#if QT_VERSION < 0x050000
#include <QUrl>
#endif
#include <QMimeData>
#include <QStyle>
#include <QSettings>
#include <QDesktopWidget>
#include <QListWidget>

#include <iostream>

#include <QFontDatabase>

int MENU_OFFSET = 0;

const QString BitcoinGUI::DEFAULT_WALLET = "~Default";

BitcoinGUI::BitcoinGUI(QWidget *parent) :
    QMainWindow(parent),
    clientModel(0),
    encryptWalletAction(0),
    changePassphraseAction(0),
    aboutQtAction(0),
    trayIcon(0),
    notificator(0),
    rpcConsole(0),
    prevBlocks(0)
{
    restoreWindowGeometry();
    setWindowTitle(tr("ZettelKasten") + " - " + tr("Wallet"));
#ifndef Q_OS_MAC
    QApplication::setWindowIcon(QIcon(":icons/bitcoin"));
    setWindowIcon(QIcon(":icons/bitcoin"));
#else
    setUnifiedTitleAndToolBarOnMac(true);
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
#endif


    int id = QFontDatabase::addApplicationFont(":/ui/fonts/opensans-regular");
    id = QFontDatabase::addApplicationFont(":/ui/fonts/opensans-semibold");
    id = QFontDatabase::addApplicationFont(":/ui/fonts/opensans-bold");
    id = QFontDatabase::addApplicationFont(":/ui/fonts/st-charles-1");
    id = QFontDatabase::addApplicationFont(":/ui/fonts/st-charles-2");


    setMinimumWidth(1000);
    setMinimumHeight(600);

    // Create wallet frame and make it the central widget
    walletFrame = new WalletFrame(this);
    //setCentralWidget(walletFrame);
    walletFrame->setMinimumWidth(800);
    walletFrame->setMinimumHeight(400);

    // Accept D&D of URIs
    setAcceptDrops(true);

    // Create actions for the toolbar, menu bar and tray/dock icon
    // Needs walletFrame to be initialized
    createActions();

    // Create application menu bar
    createMenuBar();

    // Create the toolbars
    //createToolBars();

    // Create system tray icon and notification
    createTrayIcon();

    // Create status bar
    statusBar();

    // Create header
    createHeader();

    // Create new Toolbar
    createToolBar();

    // Create sidebar
    createSideBar();

    // Create footer
    createFooter();

    // Status bar notification icons
    QFrame *frameBlocks = new QFrame();
    frameBlocks->setContentsMargins(0,0,0,0);
    frameBlocks->setMinimumWidth(56);
    frameBlocks->setMaximumWidth(56);
    QHBoxLayout *frameBlocksLayout = new QHBoxLayout(frameBlocks);
    frameBlocksLayout->setContentsMargins(3,0,3,0);
    frameBlocksLayout->setSpacing(3);
    labelEncryptionIcon = new QLabel();
    frameBlocksLayout->addWidget(labelEncryptionIcon);
    frameBlocksLayout->addStretch();

    // Progress bar and label for blocks download
    progressBarLabel = new QLabel();
    progressBarLabel->setVisible(false);
    progressBar = new QProgressBar();
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setVisible(false);

    // Override style sheet for progress bar for styles that have a segmented progress bar,
    // as they make the text unreadable (workaround for issue #1071)
    // See https://qt-project.org/doc/qt-4.8/gallery.html
    QString curStyle = QApplication::style()->metaObject()->className();
    if(curStyle == "QWindowsStyle" || curStyle == "QWindowsXPStyle")
    {
        progressBar->setStyleSheet("QProgressBar { background-color: #e8e8e8; border: 1px solid grey; border-radius: 7px; padding: 1px; text-align: center; } QProgressBar::chunk { background: QLinearGradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #FF8000, stop: 1 orange); border-radius: 7px; margin: 0px; }");
    }

    syncIconMovie = new QMovie(":/movies/update_spinner", "mng", this);

    rpcConsole = new RPCConsole(this);
    connect(openRPCConsoleAction, SIGNAL(triggered()), rpcConsole, SLOT(showConsole()));
    connect(openInfoAction, SIGNAL(triggered()), rpcConsole, SLOT(showInfo()));

    // Install event filter to be able to catch status tip events (QEvent::StatusTip)
    this->installEventFilter(this);

    // Initially wallet actions should be disabled
    setWalletActionsEnabled(false);

    //init pos adjustment
    walletFrame->move(200,MENU_OFFSET+uiHeader->height()+uiToolBar->height());

    uiHeader->raise();

    statusBar()->raise();

    startTimer(1500);
}

BitcoinGUI::~BitcoinGUI()
{
    saveWindowGeometry();
    if(trayIcon) // Hide tray icon, as deleting will let it linger until quit (on Ubuntu)
        trayIcon->hide();
#ifdef Q_OS_MAC
    delete appMenuBar;
    MacDockIconHandler::instance()->setMainWindow(NULL);
#endif
}

void BitcoinGUI::createActions()
{
    QActionGroup *tabGroup = new QActionGroup(this);

    timelineAction = new QAction(QIcon(":/icons/timeline"), tr("&Timeline"), this);
    timelineAction->setStatusTip(tr("Show timeline of blockchain"));
    timelineAction->setToolTip(timelineAction->statusTip());
    timelineAction->setCheckable(true);
    timelineAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(timelineAction);

    sendCoinsAction = new QAction(QIcon(":/icons/send"), tr("&Send"), this);
    sendCoinsAction->setStatusTip(tr("Send coins to a ZettelKasten address"));
    sendCoinsAction->setToolTip(sendCoinsAction->statusTip());
    sendCoinsAction->setCheckable(true);
    sendCoinsAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_2));
    tabGroup->addAction(sendCoinsAction);

    receiveCoinsAction = new QAction(QIcon(":/icons/receiving_addresses"), tr("&Receive"), this);
    receiveCoinsAction->setStatusTip(tr("Show the list of addresses for receiving payments"));
    receiveCoinsAction->setToolTip(receiveCoinsAction->statusTip());
    receiveCoinsAction->setCheckable(true);
    receiveCoinsAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_3));
    tabGroup->addAction(receiveCoinsAction);

    historyAction = new QAction(QIcon(":/icons/history"), tr("&Transactions"), this);
    historyAction->setStatusTip(tr("Browse transaction history"));
    historyAction->setToolTip(historyAction->statusTip());
    historyAction->setCheckable(true);
    historyAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_4));
    tabGroup->addAction(historyAction);

    addressBookAction = new QAction(QIcon(":/icons/address-book"), tr("&Addresses"), this);
    addressBookAction->setStatusTip(tr("Edit the list of stored addresses and labels"));
    addressBookAction->setToolTip(addressBookAction->statusTip());
    addressBookAction->setCheckable(true);
    addressBookAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_5));
    tabGroup->addAction(addressBookAction);

    miningAction = new QAction(QIcon(":/icons/tx_mined"), tr("&Mining"), this);
    miningAction->setStatusTip(tr("Mine new Zettel"));
    miningAction->setToolTip(miningAction->statusTip());
    miningAction->setCheckable(true);
    miningAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_6));
    tabGroup->addAction(miningAction);

    connect(timelineAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(timelineAction, SIGNAL(triggered()), this, SLOT(gotoTimelinePage()));
    connect(sendCoinsAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(sendCoinsAction, SIGNAL(triggered()), this, SLOT(gotoSendCoinsPage()));
    connect(receiveCoinsAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(receiveCoinsAction, SIGNAL(triggered()), this, SLOT(gotoReceiveCoinsPage()));
    connect(historyAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(historyAction, SIGNAL(triggered()), this, SLOT(gotoHistoryPage()));
    connect(addressBookAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(addressBookAction, SIGNAL(triggered()), this, SLOT(gotoAddressBookPage()));
    connect(miningAction, SIGNAL(triggered()), this, SLOT(gotoMiningPage()));

    quitAction = new QAction(QIcon(":/icons/quit"), tr("E&xit"), this);
    quitAction->setStatusTip(tr("Quit application"));
    quitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    quitAction->setMenuRole(QAction::QuitRole);
    aboutAction = new QAction(QIcon(":/icons/bitcoin"), tr("&About ZettelKasten"), this);
    aboutAction->setStatusTip(tr("Show information about ZettelKasten"));
    aboutAction->setMenuRole(QAction::AboutRole);
    aboutQtAction = new QAction(QIcon(":/trolltech/qmessagebox/images/qtlogo-64.png"), tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show information about Qt"));
    aboutQtAction->setMenuRole(QAction::AboutQtRole);
    optionsAction = new QAction(QIcon(":/icons/options"), tr("&Options..."), this);
    optionsAction->setStatusTip(tr("Modify configuration options for ZettelKasten"));
    optionsAction->setMenuRole(QAction::PreferencesRole);
    toggleHideAction = new QAction(QIcon(":/icons/bitcoin"), tr("&Show / Hide"), this);
    toggleHideAction->setStatusTip(tr("Show or hide the main Window"));

    encryptWalletAction = new QAction(QIcon(":/icons/lock_closed"), tr("&Encrypt Wallet..."), this);
    encryptWalletAction->setStatusTip(tr("Encrypt the private keys that belong to your wallet"));
    encryptWalletAction->setCheckable(true);
    changePassphraseAction = new QAction(QIcon(":/icons/key"), tr("&Change Passphrase..."), this);
    changePassphraseAction->setStatusTip(tr("Change the passphrase used for wallet encryption"));
    signMessageAction = new QAction(QIcon(":/icons/edit"), tr("Sign &message..."), this);
    signMessageAction->setStatusTip(tr("Sign messages with your ZettelKasten addresses to prove you own them"));
    verifyMessageAction = new QAction(QIcon(":/icons/transaction_0"), tr("&Verify message..."), this);
    verifyMessageAction->setStatusTip(tr("Verify messages to ensure they were signed with specified ZettelKasten addresses"));

    openRPCConsoleAction = new QAction(QIcon(":/icons/console"), tr("&Debug Console"), this);
    openRPCConsoleAction->setStatusTip(tr("Open debugging and diagnostic console"));

    openInfoAction = new QAction(QIcon(":/icons/info"), tr("&Information"), this);
    openInfoAction->setStatusTip(tr("Show debugging and diagnostic information"));

    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutClicked()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(optionsClicked()));
    connect(toggleHideAction, SIGNAL(triggered()), this, SLOT(toggleHidden()));
    connect(encryptWalletAction, SIGNAL(triggered(bool)), walletFrame, SLOT(encryptWallet(bool)));
    connect(changePassphraseAction, SIGNAL(triggered()), walletFrame, SLOT(changePassphrase()));
    connect(signMessageAction, SIGNAL(triggered()), this, SLOT(gotoSignMessageTab()));
    connect(verifyMessageAction, SIGNAL(triggered()), this, SLOT(gotoVerifyMessageTab()));

}

void BitcoinGUI::createMenuBar()
{
#ifdef Q_OS_MAC
    // Create a decoupled menu bar on Mac which stays even if the window is closed
    appMenuBar = new QMenuBar();
#else
    // Get the main window's menu bar on other platforms
    appMenuBar = menuBar();
#endif

    appMenuBar->raise();

    // Configure the menus
    QMenu *file = appMenuBar->addMenu(tr("&File"));
    file->addAction(signMessageAction);
    file->addAction(verifyMessageAction);
    file->addSeparator();
    file->addAction(quitAction);

    QMenu *settings = appMenuBar->addMenu(tr("&Settings"));
    settings->addAction(encryptWalletAction);
    settings->addAction(changePassphraseAction);
    settings->addSeparator();
    settings->addAction(optionsAction);

    QMenu *tools = appMenuBar->addMenu(tr("&Tools"));
    tools->addAction(openInfoAction);
    tools->addAction(openRPCConsoleAction);

    QMenu *help = appMenuBar->addMenu(tr("&About"));
    help->addAction(aboutAction);
    help->addAction(aboutQtAction);

    appMenuBar->isNativeMenuBar() ? MENU_OFFSET = 0 : MENU_OFFSET = appMenuBar->sizeHint().height();
}

void BitcoinGUI::createToolBars()
{
    QToolBar *toolbar = addToolBar(tr("Tabs toolbar"));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->addAction(timelineAction);
    toolbar->addAction(sendCoinsAction);
    toolbar->addAction(receiveCoinsAction);
    toolbar->addAction(historyAction);
    toolbar->addAction(addressBookAction);
    toolbar->addAction(miningAction);
}

void BitcoinGUI::createSideBar()
{
    uiSideBar = new UISideBar(this);
    uiSideBar->move(0,uiHeader->height()+MENU_OFFSET);
    uiSideBar->resize(200,1000);

    connect(uiSideBar, SIGNAL(sideBarClick(int)), this, SLOT(sideBarClick(int)));
}

void BitcoinGUI::createHeader()
{
    uiHeader = new UIHeader(this);
    uiHeader->move(0,MENU_OFFSET);
}

void BitcoinGUI::createToolBar()
{
    uiToolBar = new UIToolBar(this);

    uiToolBar->move(200,uiHeader->height()+MENU_OFFSET);
    connect(uiToolBar, SIGNAL(signalButtonSendCoins()), this, SLOT(gotoSendCoinsPage()));
    connect(uiToolBar, SIGNAL(signalButtonReceiveCoins()), this, SLOT(gotoReceiveCoinsPage()));
    connect(uiToolBar, SIGNAL(signalButtonHistoryPage()), this, SLOT(gotoHistoryPage()));
    connect(uiToolBar, SIGNAL(signalButtonAddressBookPage()), this, SLOT(gotoAddressBookPage()));
}

void BitcoinGUI::createFooter()
{
    uiFooter = new UIFooter(this);
    uiFooter->move(0,this->height()-75);
}

void BitcoinGUI::setClientModel(ClientModel *clientModel)
{
    this->clientModel = clientModel;
    if(clientModel)
    {
        // Create system tray menu (or setup the dock menu) that late to prevent users from calling actions,
        // while the client has not yet fully loaded
        createTrayIconMenu();

        // Keep up to date with client
        //setNumConnections(clientModel->getNumConnections());
        uiFooter->setNumConnections(clientModel->getNumConnections());
        connect(clientModel, SIGNAL(numConnectionsChanged(int)), uiFooter, SLOT(setNumConnections(int)));

        setNumBlocks(clientModel->getNumBlocks(), clientModel->getNumBlocksOfPeers());
        connect(clientModel, SIGNAL(numBlocksChanged(int,int)), this, SLOT(setNumBlocks(int,int)));

        // Receive and report messages from network/worker thread
        connect(clientModel, SIGNAL(message(QString,QString,unsigned int)), this, SLOT(message(QString,QString,unsigned int)));

        rpcConsole->setClientModel(clientModel);
        walletFrame->setClientModel(clientModel);
    }
}

bool BitcoinGUI::addWallet(const QString& name, WalletModel *walletModel)
{
    setWalletActionsEnabled(true);
    return walletFrame->addWallet(name, walletModel);
}

bool BitcoinGUI::setCurrentWallet(const QString& name)
{
    return walletFrame->setCurrentWallet(name);
}

void BitcoinGUI::removeAllWallets()
{
    setWalletActionsEnabled(false);
    walletFrame->removeAllWallets();
}

void BitcoinGUI::setWalletActionsEnabled(bool enabled)
{
    timelineAction->setEnabled(enabled);
    sendCoinsAction->setEnabled(enabled);
    receiveCoinsAction->setEnabled(enabled);
    historyAction->setEnabled(enabled);
    encryptWalletAction->setEnabled(enabled);
    changePassphraseAction->setEnabled(enabled);
    signMessageAction->setEnabled(enabled);
    verifyMessageAction->setEnabled(enabled);
    addressBookAction->setEnabled(enabled);
}

void BitcoinGUI::createTrayIcon()
{
#ifndef Q_OS_MAC
    trayIcon = new QSystemTrayIcon(this);

    trayIcon->setToolTip(tr("ZettelKasten client"));
    trayIcon->setIcon(QIcon(":/icons/toolbar"));
    trayIcon->show();
#endif

    notificator = new Notificator(QApplication::applicationName(), trayIcon);
}

void BitcoinGUI::createTrayIconMenu()
{
    QMenu *trayIconMenu;
#ifndef Q_OS_MAC
    // return if trayIcon is unset (only on non-Mac OSes)
    if (!trayIcon)
        return;

    trayIconMenu = new QMenu(this);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
#else
    // Note: On Mac, the dock icon is used to provide the tray's functionality.
    MacDockIconHandler *dockIconHandler = MacDockIconHandler::instance();
    dockIconHandler->setMainWindow((QMainWindow *)this);
    trayIconMenu = dockIconHandler->dockMenu();
#endif

    // Configuration of the tray icon (or dock icon) icon menu
    trayIconMenu->addAction(toggleHideAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(sendCoinsAction);
    trayIconMenu->addAction(receiveCoinsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(signMessageAction);
    trayIconMenu->addAction(verifyMessageAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(optionsAction);
    trayIconMenu->addAction(openInfoAction);
    trayIconMenu->addAction(openRPCConsoleAction);

#ifndef Q_OS_MAC // This is built-in on Mac
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
#endif
}

#ifndef Q_OS_MAC
void BitcoinGUI::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        // Click on system tray icon triggers show/hide of the main window
        toggleHideAction->trigger();
    }
}
#endif

void BitcoinGUI::saveWindowGeometry()
{
    QSettings settings;
    settings.setValue("nWindowPos", pos());
    settings.setValue("nWindowSize", size());
}

void BitcoinGUI::restoreWindowGeometry()
{
    QSettings settings;
    QPoint pos = settings.value("nWindowPos").toPoint();
    QSize size = settings.value("nWindowSize", QSize(850, 550)).toSize();
    if (!pos.x() && !pos.y())
    {
        QRect screen = QApplication::desktop()->screenGeometry();
        pos.setX((screen.width()-size.width())/2);
        pos.setY((screen.height()-size.height())/2);
    }
    resize(size);
    move(pos);
}

void BitcoinGUI::optionsClicked()
{
    if(!clientModel || !clientModel->getOptionsModel())
        return;
    OptionsDialog dlg;
    dlg.setModel(clientModel->getOptionsModel());
    dlg.exec();
}

void BitcoinGUI::aboutClicked()
{
    AboutDialog dlg;
    dlg.setModel(clientModel);
    dlg.exec();
}

void BitcoinGUI::sideBarClick(int index)
{
    if(index == 0){
        uiToolBar->clicked_button = 5;
        gotoTimelinePage();
    }
    if(index == 1){
        uiToolBar->clicked_button = 0;
        gotoSendCoinsPage();
    }
    if(index == 2){
        uiToolBar->clicked_button = 4;
        gotoMiningPage();
    }
}

void BitcoinGUI::gotoTimelinePage()
{
    uiToolBar->switchToToolBar(0);
    if (walletFrame) walletFrame->gotoTimelinePage();
}

void BitcoinGUI::gotoHistoryPage()
{
    uiToolBar->switchToToolBar(1);
    if (walletFrame) walletFrame->gotoHistoryPage();
}

void BitcoinGUI::gotoAddressBookPage()
{
    uiToolBar->switchToToolBar(1);
    if (walletFrame) walletFrame->gotoAddressBookPage();
}

void BitcoinGUI::gotoReceiveCoinsPage()
{
    uiToolBar->switchToToolBar(1);
    if (walletFrame) walletFrame->gotoReceiveCoinsPage();
}

void BitcoinGUI::gotoSendCoinsPage(QString addr)
{
    uiToolBar->switchToToolBar(1);
    if (walletFrame) walletFrame->gotoSendCoinsPage(addr);
}

void BitcoinGUI::gotoSignMessageTab(QString addr)
{
    if (walletFrame) walletFrame->gotoSignMessageTab(addr);
}

void BitcoinGUI::gotoVerifyMessageTab(QString addr)
{
    if (walletFrame) walletFrame->gotoVerifyMessageTab(addr);
}

void BitcoinGUI::gotoMiningPage()
{
    uiToolBar->switchToToolBar(2);
    if (walletFrame) walletFrame->gotoMiningPage();
}

void BitcoinGUI::setNumBlocks(int count, int nTotalBlocks)
{
    // Prevent orphan statusbar messages (e.g. hover Quit in main menu, wait until chain-sync starts -> garbelled text)
    statusBar()->clearMessage();

    QStringList str_list;

    // if(fDebug){ printf("NumBlocks: %d : %d\n", count, nTotalBlocks); }
    if(GetBoolArg("-chart", true) && count > 0 && nTotalBlocks > 0)
    {
        walletFrame->updatePlot();



        uiFooter->updateDataLabels();//str_list);
    }

    //progressBarLabel->setStyleSheet("QLabel {color: #FFFFFF;}");

    QString str_blockSource;
    // Acquire current block source
    enum BlockSource blockSource = clientModel->getBlockSource();
    switch (blockSource) {
    case BLOCK_SOURCE_NETWORK:
        str_blockSource = QString(tr("Synchronizing with network..."));
        break;
    case BLOCK_SOURCE_DISK:
        str_blockSource = QString(tr("Importing blocks from disk..."));
        break;
    case BLOCK_SOURCE_REINDEX:
        str_blockSource = QString(tr("Reindexing blocks on disk..."));
        break;
    case BLOCK_SOURCE_NONE:
        // Case: not Importing, not Reindexing and no network connection
        str_blockSource = QString(tr("No block source available..."));
        break;
    }

    str_list << str_blockSource;
    QString tooltip;

    QDateTime lastBlockDate = clientModel->getLastBlockDate();
    QDateTime currentDate = QDateTime::currentDateTime();
    int secs = lastBlockDate.secsTo(currentDate);

    if(count < nTotalBlocks)
    {
        tooltip = tr("Processed %1 of %2 (estimated) blocks of transaction history.").arg(count).arg(nTotalBlocks);
    }
    else
    {
        tooltip = tr("Processed %1 blocks of transaction history.").arg(count);
    }

    // Set icon state: spinning if catching up, tick otherwise
    if(secs < 90*60 && count >= nTotalBlocks)
    {
        tooltip = tr("Up to date") + QString(".<br>") + tooltip;
        //labelBlocksIcon->setPixmap(QIcon(":/icons/synced").pixmap(STATUSBAR_ICONSIZE, STATUSBAR_ICONSIZE));

        walletFrame->showOutOfSyncWarning(false);

        progressBarLabel->setVisible(false);
        progressBar->setVisible(false);
    }
    else
    {
        // Represent time from last generated block in human readable text
        QString timeBehindText;
        if(secs < 48*60*60)
        {
            timeBehindText = tr("%n hour(s)","",secs/(60*60));
        }
        else if(secs < 14*24*60*60)
        {
            timeBehindText = tr("%n day(s)","",secs/(24*60*60));
        }
        else
        {
            timeBehindText = tr("%n week(s)","",secs/(7*24*60*60));
        }

        str_list << QString(tr("%1 behind").arg(timeBehindText));

        tooltip = tr("Catching up...") + QString("<br>") + tooltip;
       // labelBlocksIcon->setMovie(syncIconMovie);
        if(count != prevBlocks)
            syncIconMovie->jumpToNextFrame();
        prevBlocks = count;

        walletFrame->showOutOfSyncWarning(true);

        tooltip += QString("<br>");
        tooltip += tr("Last received block was generated %1 ago.").arg(timeBehindText);
        tooltip += QString("<br>");
        tooltip += tr("Transactions after this will not yet be visible.");
    }

    // Don't word-wrap this (fixed-width) tooltip
    tooltip = QString("<nobr>") + tooltip + QString("</nobr>");

    uiFooter->setProgressInfo(str_list, count, nTotalBlocks, secs, blockSource);
    walletFrame->passSyncData(str_list, count, nTotalBlocks, secs);
}

void BitcoinGUI::message(const QString &title, const QString &message, unsigned int style, bool *ret)
{
    QString strTitle = tr("ZettelKasten"); // default title
    // Default to information icon
    int nMBoxIcon = QMessageBox::Information;
    int nNotifyIcon = Notificator::Information;

    // Override title based on style
    QString msgType;
    switch (style) {
    case CClientUIInterface::MSG_ERROR:
        msgType = tr("Error");
        break;
    case CClientUIInterface::MSG_WARNING:
        msgType = tr("Warning");
        break;
    case CClientUIInterface::MSG_INFORMATION:
        msgType = tr("Information");
        break;
    default:
        msgType = title; // Use supplied title
    }
    if (!msgType.isEmpty())
        strTitle += " - " + msgType;

    // Check for error/warning icon
    if (style & CClientUIInterface::ICON_ERROR) {
        nMBoxIcon = QMessageBox::Critical;
        nNotifyIcon = Notificator::Critical;
    }
    else if (style & CClientUIInterface::ICON_WARNING) {
        nMBoxIcon = QMessageBox::Warning;
        nNotifyIcon = Notificator::Warning;
    }

    // Display message
    if (style & CClientUIInterface::MODAL) {
        // Check for buttons, use OK as default, if none was supplied
        QMessageBox::StandardButton buttons;
        if (!(buttons = (QMessageBox::StandardButton)(style & CClientUIInterface::BTN_MASK)))
            buttons = QMessageBox::Ok;

        QMessageBox mBox((QMessageBox::Icon)nMBoxIcon, strTitle, message, buttons);
        int r = mBox.exec();
        if (ret != NULL)
            *ret = r == QMessageBox::Ok;
    }
    else
        notificator->notify((Notificator::Class)nNotifyIcon, strTitle, message);
}

void BitcoinGUI::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
#ifndef Q_OS_MAC // Ignored on Mac
    if(e->type() == QEvent::WindowStateChange)
    {
        if(clientModel && clientModel->getOptionsModel()->getMinimizeToTray())
        {
            QWindowStateChangeEvent *wsevt = static_cast<QWindowStateChangeEvent*>(e);
            if(!(wsevt->oldState() & Qt::WindowMinimized) && isMinimized())
            {
                QTimer::singleShot(0, this, SLOT(hide()));
                e->ignore();
            }
        }
    }
#endif
}

void BitcoinGUI::closeEvent(QCloseEvent *event)
{
    if(clientModel)
    {
#ifndef Q_OS_MAC // Ignored on Mac
        if(!clientModel->getOptionsModel()->getMinimizeToTray() &&
                !clientModel->getOptionsModel()->getMinimizeOnClose())
        {
            QApplication::quit();
        }
#endif
    }
    QMainWindow::closeEvent(event);
}

void BitcoinGUI::askFee(qint64 nFeeRequired, bool *payFee)
{
    QString strMessage = tr("This transaction is over the size limit. You can still send it for a fee of %1, "
                            "which goes to the nodes that process your transaction and helps to support the network. "
                            "Do you want to pay the fee?").arg(BitcoinUnits::formatWithUnit(BitcoinUnits::BTC, nFeeRequired));
    QMessageBox::StandardButton retval = QMessageBox::question(
                this, tr("Confirm transaction fee"), strMessage,
                QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes);
    *payFee = (retval == QMessageBox::Yes);
}

void BitcoinGUI::incomingTransaction(const QString& date, int unit, qint64 amount, const QString& type, const QString& address)
{
    // On new transaction, make an info balloon
    message((amount)<0 ? tr("Sent transaction") : tr("Incoming transaction"),
            tr("Date: %1\n"
               "Amount: %2\n"
               "Type: %3\n"
               "Address: %4\n")
            .arg(date)
            .arg(BitcoinUnits::formatWithUnit(unit, amount, true))
            .arg(type)
            .arg(address), CClientUIInterface::MSG_INFORMATION);
}



void BitcoinGUI::dragEnterEvent(QDragEnterEvent *event)
{
    // Accept only URIs
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void BitcoinGUI::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        int nValidUrisFound = 0;
        QList<QUrl> uris = event->mimeData()->urls();
        foreach(const QUrl &uri, uris)
        {
            if (walletFrame->handleURI(uri.toString()))
                nValidUrisFound++;
        }

        // if valid URIs were found
        if (nValidUrisFound)
            walletFrame->gotoSendCoinsPage();
        else
            message(tr("URI handling"), tr("URI can not be parsed! This can be caused by an invalid ZettelKasten address or malformed URI parameters."),
                    CClientUIInterface::ICON_WARNING);
    }

    event->acceptProposedAction();
}

bool BitcoinGUI::eventFilter(QObject *object, QEvent *event)
{
    // Catch status tip events
    if (event->type() == QEvent::StatusTip)
    {
        // Prevent adding text from setStatusTip(), if we currently use the status bar for displaying other stuff
        if (progressBarLabel->isVisible() || progressBar->isVisible())
            return true;
    }
    return QMainWindow::eventFilter(object, event);
}

void BitcoinGUI::timerEvent(QTimerEvent *event)
{

}

void BitcoinGUI::resizeEvent(QResizeEvent *event)
{
    uiHeader->resize( this->width(), uiHeader->height());
    uiToolBar->resize( this->width()-200, uiToolBar->height());
    uiSideBar->resize(200,this->height()-MENU_OFFSET-uiHeader->height()-75);

    walletFrame->resize(this->width()-200, this->height()-MENU_OFFSET-uiHeader->height()-uiToolBar->height()-75);

    uiFooter->move(0,this->height()-75);
    uiFooter->resize(this->width(),uiFooter->height());
}

void BitcoinGUI::handleURI(QString strURI)
{
    // URI has to be valid
    if (!walletFrame->handleURI(strURI))
        message(tr("URI handling"), tr("URI can not be parsed! This can be caused by an invalid ZettelKasten address or malformed URI parameters."),
                CClientUIInterface::ICON_WARNING);
}

void BitcoinGUI::setEncryptionStatus(int status)
{
    switch(status)
    {
    case WalletModel::Unencrypted:
        labelEncryptionIcon->hide();
        encryptWalletAction->setChecked(false);
        changePassphraseAction->setEnabled(false);
        encryptWalletAction->setEnabled(true);
        break;
    case WalletModel::Unlocked:
        labelEncryptionIcon->show();
        labelEncryptionIcon->setPixmap(QIcon(":/icons/lock_open").pixmap(STATUSBAR_ICONSIZE,STATUSBAR_ICONSIZE));
        labelEncryptionIcon->setToolTip(tr("Wallet is <b>encrypted</b> and currently <b>unlocked</b>"));
        encryptWalletAction->setChecked(true);
        changePassphraseAction->setEnabled(true);
        encryptWalletAction->setEnabled(false); // TODO: decrypt currently not supported
        break;
    case WalletModel::Locked:
        labelEncryptionIcon->show();
        labelEncryptionIcon->setPixmap(QIcon(":/icons/lock_closed").pixmap(STATUSBAR_ICONSIZE,STATUSBAR_ICONSIZE));
        labelEncryptionIcon->setToolTip(tr("Wallet is <b>encrypted</b> and currently <b>locked</b>"));
        encryptWalletAction->setChecked(true);
        changePassphraseAction->setEnabled(true);
        encryptWalletAction->setEnabled(false); // TODO: decrypt currently not supported
        break;
    }
}

void BitcoinGUI::showNormalIfMinimized(bool fToggleHidden)
{
    // activateWindow() (sometimes) helps with keyboard focus on Windows
    if (isHidden())
    {
        show();
        activateWindow();
    }
    else if (isMinimized())
    {
        showNormal();
        activateWindow();
    }
    else if (GUIUtil::isObscured(this))
    {
        raise();
        activateWindow();
    }
    else if(fToggleHidden)
        hide();
}

void BitcoinGUI::toggleHidden()
{
    showNormalIfMinimized(true);
}

void BitcoinGUI::detectShutdown()
{
    if (ShutdownRequested())
        QMetaObject::invokeMethod(QCoreApplication::instance(), "quit", Qt::QueuedConnection);
}
