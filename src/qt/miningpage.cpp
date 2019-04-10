#include "miningpage.h"
#include "ui_miningpage.h"
#include "main.h"
#include "util.h"
#include "bitcoinrpc.h"
#include "walletmodel.h"

#include <boost/thread.hpp>
#include <stdio.h>

extern json_spirit::Value GetNetworkHashPS(int lookup, int height);

MiningPage::MiningPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiningPage),
    hasMiningprivkey(false)
{
    ui->setupUi(this);

    int nThreads = boost::thread::hardware_concurrency();

    int nUseThreads = GetArg("-genproclimit", -1);
	if (nUseThreads < 0){
         nUseThreads = nThreads;
	}

    std::string PrivAddress = GetArg("-miningprivkey", "");
    if (!PrivAddress.empty())
    {
        CBitcoinSecret Secret;
        Secret.SetString(PrivAddress);
        if (Secret.IsValid())
        {
            CBitcoinAddress Address;
            Address.Set(Secret.GetKey().GetPubKey().GetID());
            ui->labelAddress->setText(QString("All mined coins will go to to %1").arg(Address.ToString().c_str()));
            hasMiningprivkey = true;
        }
    }

    ui->sliderCores->setMinimum(0);
    ui->sliderCores->setMaximum(nThreads);
    ui->sliderCores->setValue(nUseThreads);
    ui->labelNCores->setText(QString("%1").arg(nUseThreads));

    connect(ui->sliderCores, SIGNAL(valueChanged(int)), this, SLOT(changeNumberOfCores(int)));
    connect(ui->pushSwitchMining, SIGNAL(clicked()), this, SLOT(switchMining()));

    updateUI();
    startTimer(1500);
}

MiningPage::~MiningPage()
{
    delete ui;
}

void MiningPage::setModel(WalletModel *model)
{
    this->model = model;
}

void MiningPage::updateUI()
{
    int nThreads = boost::thread::hardware_concurrency();

    int nUseThreads = GetArg("-genproclimit", -1);
    if (nUseThreads < 0)
        nUseThreads = nThreads;


    ui->labelNCores->setText(QString("%1").arg(nUseThreads));
    ui->pushSwitchMining->setText(GetBoolArg("-gen", false)? tr("Stop mining") : tr("Start mining"));
}

void MiningPage::restartMining(bool fGenerate)
{
    int nThreads = ui->sliderCores->value();

    mapArgs["-genproclimit"] = QString("%1").arg(nThreads).toUtf8().data();

    // unlock wallet before mining

#ifndef __linux__
    if (fGenerate && !hasMiningprivkey && !unlockContext.get())
    {
        this->unlockContext.reset(new WalletModel::UnlockContext(model->requestUnlock()));
        if (!unlockContext->isValid())
        {
            unlockContext.reset(NULL);
            return;
        }
    }
#endif

    json_spirit::Array Args;
    Args.push_back(fGenerate);
    Args.push_back(nThreads);
    setgenerate(Args, false);

    // lock wallet after mining
    if (!fGenerate && !hasMiningprivkey)
        unlockContext.reset(NULL);

    updateUI();
}

void MiningPage::changeNumberOfCores(int i)
{
    restartMining(GetBoolArg("-gen"));
}

void MiningPage::switchMining()
{
    restartMining(!GetBoolArg("-gen"));
}

static QString formatTimeInterval(CBigNum t)
{
    enum  EUnit { YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, NUM_UNITS };

    const int SecondsPerUnit[NUM_UNITS] =
    {
        31556952, // average number of seconds in gregorian year
        31556952/12, // average number of seconds in gregorian month
        24*60*60, // number of seconds in a day
        60*60, // number of seconds in an hour
        60, // number of seconds in a minute
        1
    };

    const char* UnitNames[NUM_UNITS] =
    {
        "year",
        "month",
        "day",
        "hour",
        "minute",
        "second"
    };

    if (t > 0xFFFFFFFF)
    {
        t /= SecondsPerUnit[YEAR];
        return QString("%1 years").arg(t.ToString(10).c_str());
    }
    else
    {
        unsigned int t32 = t.getuint();

        int Values[NUM_UNITS];
        for (int i = 0; i < NUM_UNITS; i++)
        {
            Values[i] = t32/SecondsPerUnit[i];
            t32 %= SecondsPerUnit[i];
        }

        int FirstNonZero = 0;
        while (FirstNonZero < NUM_UNITS && Values[FirstNonZero] == 0)
            FirstNonZero++;

        QString TimeStr;
        for (int i = FirstNonZero; i < std::min(FirstNonZero + 3, (int)NUM_UNITS); i++)
        {
            int Value = Values[i];
            TimeStr += QString("%1 %2%3 ").arg(Value).arg(UnitNames[i]).arg((Value == 1)? "" : "s"); // FIXME: this is English specific
        }
        return TimeStr;
    }
}

static QString formatHashrate(int64 n)
{
    if (n == 0)
        return "0 H/s";

    int i = (int)floor(log(n)/log(1000));
    float v = n*pow(1000.0f, -i);

    QString prefix = "";
    if (i >= 1 && i < 9)
        prefix = " kMGTPEZY"[i];

    return QString("%1 %2H/s").arg(v, 0, 'f', 2).arg(prefix);
}

void MiningPage::timerEvent(QTimerEvent *)
{
	int64 NetworkHashrate = GetNetworkHashPS(120, -1).get_int64();
	int64 Hashrate = GetBoolArg("-gen")? gethashespersec(json_spirit::Array(), false).get_int64() : 0;

	QString NextBlockTime;
	if (Hashrate == 0)
		NextBlockTime = QChar(L'∞');
	else
	{
		CBigNum Target;
		Target.SetCompact(pindexBest->nBits);
		CBigNum ExpectedTime = (CBigNum(1) << 256)/(Target*Hashrate);
		NextBlockTime = formatTimeInterval(ExpectedTime);
	}

	ui->labelNethashrate->setText(formatHashrate(NetworkHashrate));
	ui->labelYourHashrate->setText(formatHashrate(Hashrate));
	ui->labelNextBlock->setText(NextBlockTime);



	uint nHeight = pindexBest->nHeight+1;

	ui->label_SDKPRAB_1->setVisible(false);
	ui->label_SDKPRAB_2->setVisible(false);
	ui->label_SDKPRAB_3->setVisible(false);
	ui->label_SDKPRAB_4->setVisible(false);
	ui->label_SDKPRAB_5->setVisible(false);
	ui->label_SDKPRAB_6->setVisible(false);
	ui->label_SDKPRAB_7->setVisible(false);
	ui->label_SDKPRAB_8->setVisible(false);

	ui->label_hashalgo->setText("mining block with height "+QString::number(nHeight)+" using SpreadDoubleKetchup");


	ui->label_SDKPRABSPC_1->setVisible(false);
	ui->label_SDKPRABSPC_2->setVisible(false);

    if(nHeight >= SDKPGAB_START_HEIGHT){
        ui->label_SDKPRAB_1->setVisible(true);
        ui->label_SDKPRAB_2->setVisible(true);
        ui->label_SDKPRAB_3->setVisible(true);
        ui->label_SDKPRAB_4->setVisible(true);
        ui->label_SDKPRAB_5->setVisible(true);
        ui->label_SDKPRAB_6->setVisible(true);
        ui->label_SDKPRAB_7->setVisible(true);
        ui->label_SDKPRAB_8->setVisible(true);

        ui->label_hashalgo->setText("mining block with height "+QString::number(nHeight)+" using SpreadDoubleKetchupPrimeGradeABeef");

        FirstBytesForSDKPGAB first_bytes = GetFirstBytesForSDKPGABFromHash(pindexBest->GetBlockHash());
        ui->label_n2->setText(QString::number(first_bytes.n2));
        ui->label_n3->setText(QString::number(first_bytes.n3));
        ui->label_n5->setText(QString::number(first_bytes.n5));
        ui->label_n7->setText(QString::number(first_bytes.n7));

        ABCBytesForSDKPGAB bytes;
        bytes = GetABCBytesForSDKPGABFromHash(pindexBest->GetBlockHash());
        ui->label_A->setText(QString::number(bytes.A));
        ui->label_B->setText(QString::number(bytes.B));
        ui->label_C->setText(QString::number(bytes.C));
        ui->label_parity->setText((nHeight%2==0)?"even":"odd");
    }

    if(nHeight >= SDKPGABSPC_START_HEIGHT){
        ui->label_SDKPRABSPC_1->setVisible(true);
        ui->label_SDKPRABSPC_2->setVisible(true);
    }

    if(nHeight >= SDKPGABSPC_START_HEIGHT && nHeight < SDKPGABSPCSSWS_START_HEIGHT){
        ui->label_hashalgo->setText("mining block with height "+QString::number(nHeight)+" using SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheese");

        ui->label_inputsize_1->setText("185 + "+QString::number(SDKPGABSPC_sinetable[nHeight%64]));
        ui->label_inputsize_2->setText("64 + "+QString::number(64-SDKPGABSPC_sinetable[nHeight%64]));
    }

    if(nHeight >= SDKPGABSPCSSWS_START_HEIGHT){
        ui->label_hashalgo->setText("mining block with height "+QString::number(nHeight)+" using SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheeseSomethingSomethingWordSalad");

        ui->label_inputsize_1->setText("185 + "+QString::number(SDKPGABSPC_sinetable[nHeight%64]));
        ui->label_inputsize_2->setText("185 + "+QString::number(64-SDKPGABSPC_sinetable[nHeight%64]));


        if(nHeight >= SDKPGABSPCSSWS_START_HEIGHT +1){
            CBlockIndex* pindex = pindexBest;
            CBlockIndex* itr = pindex;
            if(itr != NULL){

                std::string wordsalad = "";

                uint32_t prev_height = pindexBest->GetBlockHeader().nHeight;

                ABCBytesForSDKPGAB bytes = GetABCBytesForSDKPGABFromHash(pindexBest->pprev->GetBlockHash());

                uint32_t SDKPGABSPC_sinetable_pos = prev_height%64;

                CBufferStream<185> Header = pindexBest->GetBlockHeader().SerializeHeaderForHash2();

                wordsalad = GetWordSalad_SDKPGABSPCSSWS(Header.begin(), Header.end(),
                                            (prev_height%2 == 0),
                                            bytes.A, bytes.B,
                                            SDKPGABSPC_sinetable_pos);

                ui->label_wordsalad_title->setText("Block #"+QString::number(prev_height)+" wordsalad:");
                ui->label_wordsalad->setText(wordsalad.c_str());
            }
        }
    }

	if(nHeight >= SDKPGABSPCSSWSSBP_START_HEIGHT){
		ui->label_hashalgo->setText("mining block with height "+QString::number(nHeight)+" using SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheeseSomethingSomethingWordSaladSenoritaBonitaPepita");

		ui->label_inputsize_1->setText("185 + "+QString::number(SDKPGABSPC_sinetable[nHeight%64]));
		ui->label_inputsize_2->setText("185 + 32 + "+QString::number(64-SDKPGABSPC_sinetable[nHeight%64]));

		ui->label_pepita_title->setText("hashPrevBlock keypair:");

		uint256 hashPrevBlock = pindexBest->GetBlockHash();
		ui->label_pepita_privkey->setText(hashPrevBlock.GetHex().c_str());

		uint256 pubkey_hashPrevBlock;

		pubkey_hashPrevBlock = SDKPGABSPCSSWSSBP_GetPublicKeyFromPrivateKey(hashPrevBlock);

		ui->label_pepita_pubkey->setText(pubkey_hashPrevBlock.GetHex().c_str());
	}

}
