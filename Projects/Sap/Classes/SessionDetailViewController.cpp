
#include "SessionDetailViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FServerTime.h"
#include "SurveyViewController.h"
#include "ConstData/ConstRect.h"
#include "FNoticeManager.h"
#include <stdio.h>
#include <time.h>

#define TIME_GAPE   (60 * 10)

SessionDetailViewController::SessionDetailViewController(sessionMsg &msg)
: m_msg(&msg)
, m_surveyButtonView(NULL)
, m_surveyTimeLabel(NULL)
, m_storeBtnImage(NULL)
, m_likeBtnImage(NULL)
, m_likeNumLabel(NULL)
, m_isSurveyed(false)
, p_alertView(NULL)
, p_pLoading(NULL)
, m_surveyBtn(NULL)
, m_surveyBtnLabel1(NULL)
, m_surveyBtnLabel2(NULL)
, m_surveyBtnLabel3(NULL)
{
	m_detailMsg.m_sessionId = -1;
}

SessionDetailViewController::~SessionDetailViewController()
{

}

void SessionDetailViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    CAScheduler::schedule(schedule_selector(SessionDetailViewController::adjustSurveyBtn), this, 1);//, true, 3000);

    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect((0), (0), m_winSize.width, (120)));
    this->getView()->addSubview(sView);
    
    // header back button
    CAButton* button = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect((20), (20), (80), (80)));
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    // header title
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, (80), m_winSize.width, (50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(SAP_TITLE_FONT_SIZE);
    label->setText("Session");
    label->setFontName(SAP_FONT_ARIAL);
    sView->addSubview(label);
    
//    initView();
	
    /*
	if (m_detailMsg.m_sessionId == -1)
	{
		requestMsg();
	}
	else
	{
		initView();
	}
     */
    requestMsg();
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SessionDetailViewController::initView()
{
	if (m_detailMsg.m_sessionId == -1)
	{
		showAlert();
		return;
	}
	//////////////////////
	//CommonUrlImageView* m_urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
	////createWithFrame(DRect((30), (40), (80), (80)));
	//m_urlImageView->setFrame(DRect((30), (40) + (120), (80), (80)));
	////m_urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	//m_urlImageView->setImage(CAImage::create("common/bg.png"));
	//m_urlImageView->setUrl(m_detailMsg.m_speaker[0].iconUrl);
	//this->getView()->addSubview(m_urlImageView);
    DRect tempRect;
    int yHight = (120);
    
    // scroll view
    CAScrollView* scrollView = CAScrollView::createWithFrame(DRect(0, yHight, m_winSize.width, 2000));//m_winSize.height * 1.5));
    scrollView->setHorizontalScrollEnabled(false);
    scrollView->setVerticalScrollEnabled(true);
    scrollView->setBounceHorizontal(false);
    scrollView->setBounds(DRect(0, 0, m_winSize.width, m_winSize.height - yHight * 2));
    scrollView->setAnchorPoint(DPoint(0.f, 0.f));
    scrollView->setShowsHorizontalScrollIndicator(false);
    scrollView->setShowsVerticalScrollIndicator(true);
    this->getView()->addSubview(scrollView);

    // header title
    yHight = (40);
    CALabel* m_titleLabel = CALabel::createWithFrame(DRect((40), yHight, m_winSize.width - (80), (500)));
    /*
    if (m_msg->m_title.size() >= 60) {
        yHight += 120;
    } else if (m_msg->m_title.size() >= 30) {
        yHight += 80;
    } else {
        yHight += 40;
    }
     */
	m_titleLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
	m_titleLabel->setTextAlignment(CATextAlignmentLeft);
	m_titleLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_titleLabel->setTextAlignment(CATextAlignmentLeft);
	m_titleLabel->setFontSize((30));
	m_titleLabel->setText(m_msg->m_title);
	scrollView->addSubview(m_titleLabel);
    
    //CCLog("session line : %d", m_msg->m_title.size() / 30);
    yHight += (m_msg->m_title.size() / 30) * (50);
                                              
    yHight += (60);

    // time
	CALabel* m_timeLabel = CALabel::createWithFrame(DRect((40), yHight, m_winSize.width - (80), (30)));
	m_timeLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
	m_timeLabel->setTextAlignment(CATextAlignmentLeft);
	//m_timeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	m_timeLabel->setFontSize((28));
	m_timeLabel->setText(crossapp_format_string("%s - %s", timeToString(m_msg->m_startTime).c_str(), timeToString(m_msg->m_endTime).c_str()));
	scrollView->addSubview(m_timeLabel);
    
    // session id button
    CAView* sessionidView = CAView::createWithFrame(DRect(m_winSize.width / 2 - (50), yHight - (5), (150), (35)));
    sessionidView->setColor(ccc4(0x99, 0x99, 0x99, 0xff));//(CAImage::create("common/dgray_bg.png"));
    //sessionidView->setColor(ccc4(0x99, 0x99, 0x99, 0x0));
    //CAImageView * sessionidView = CAImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    //sessionidView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    //sessionId->setBackgroundViewForState(CAControlStateAll, sessionidView);
    //sessionId->setControlState(CAControlStateDisabled);
    scrollView->addSubview(sessionidView);
    
    // session id button label
    CALabel* btnLabel = CALabel::createWithFrame(DRect((0), (0), (150), (35)));
    btnLabel->setColor(CAColor_white);
    btnLabel->setTextAlignment(CATextAlignmentCenter);
    btnLabel->setFontSize((25));
    btnLabel->setText(crossapp_format_string("SID : %d", m_msg->m_sessionId));
    sessionidView->addSubview(btnLabel);
    
    yHight += (30) + (20);
    
    // format
    CALabel* fromatLabel = CALabel::createWithFrame(DRect((40), yHight, m_winSize.width - (80), (28)));
    fromatLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
    fromatLabel->setTextAlignment(CATextAlignmentLeft);
    fromatLabel->setFontSize((24));
    fromatLabel->setText(crossapp_format_string("Track : %s", m_msg->m_track.c_str()));
    scrollView->addSubview(fromatLabel);
    
    yHight += (25) + (20);
    
    // track
    CALabel* trackLabel = CALabel::createWithFrame(DRect((40), yHight, m_winSize.width - (80), (28)));
    trackLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
    trackLabel->setTextAlignment(CATextAlignmentLeft);
    trackLabel->setFontSize((24));
    trackLabel->setText(crossapp_format_string("Format : %s", m_msg->m_format.c_str()));
    scrollView->addSubview(trackLabel);
    
    yHight += (25) + (20);
    
    // location
	CALabel* timeLabel = CALabel::createWithFrame(DRect((40), yHight, m_winSize.width - (80), (35)));
	timeLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
	timeLabel->setTextAlignment(CATextAlignmentLeft);
	timeLabel->setFontSize((24));
	timeLabel->setText(crossapp_format_string("Location: %s", m_msg->m_location.c_str()));
	scrollView->addSubview(timeLabel);

    yHight += (30) + (50);

    // speaker
    size_t speakerCnt = m_detailMsg.m_speaker.size();
    if (speakerCnt > 0) {
        CALabel* label = CALabel::createWithFrame(DRect((40), yHight, m_winSize.width - (80), (35)));
        label->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
        label->setTextAlignment(CATextAlignmentLeft);
        label->setFontSize((30));
        label->setText("Speaker");
        scrollView->addSubview(label);
        
        yHight += (35) + (20);
        
        int speakWidth = 0;
        if (speakerCnt > 3) {
            speakWidth = (m_winSize.width - (30 * 2)) / speakerCnt;
        } else {
            speakWidth += 120;
        }
        for (int i = 0; i < speakerCnt; i++)
        {
            tempRect = ConstRect::getSessionDetailSpeakerLogoRect();
            tempRect.origin.y = yHight;
            tempRect.origin.x = (80) + speakWidth * i + (speakWidth - tempRect.size.width) / 2;
            CommonUrlImageView* urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/head_bg.png"));
            urlImageView->setFrame(tempRect);
            //urlImageView->setImage(CAImage::create("common/bg.png"));
            urlImageView->setUrl(m_detailMsg.m_speaker[i].iconUrl);
            scrollView->addSubview(urlImageView);
            
            //tempRect = ConstRect::getSessionDetailSpeakerNameRect();
            tempRect.origin.x += tempRect.size.width / 2;
            tempRect.origin.y = tempRect.origin.y + (100);
            /*
             if (speakerCnt > 3) {
             tempRect.origin.x = (30) + speakWidth * i + (speakWidth - tempRect.size.width) / 2;
             } else {
             tempRect.origin.x = (30) + speakWidth * i + (50) * (i + 1);
             }*/
            label = CALabel::createWithCenter(DRect(tempRect.origin.x, tempRect.origin.y, (200), (30)));
            label->setColor(CAColor_gray);
            label->setTextAlignment(CATextAlignmentCenter);
            label->setFontSize((20));
            label->setText(m_detailMsg.m_speaker[i].name);
            scrollView->addSubview(label);
        }
        yHight += (100) + (50);
    }
    
    // description
    tempRect = ConstRect::getSessionDetailDescriptionRect();
    tempRect.size.width = m_winSize.width - (80);
    tempRect.origin.y = yHight;
	CALabel* m_lectureDetailLabel = CALabel::createWithFrame(tempRect);
	m_lectureDetailLabel->setTextAlignment(CATextAlignmentLeft);
    m_lectureDetailLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	m_lectureDetailLabel->setFontSize((24));
	m_lectureDetailLabel->setText(m_detailMsg.m_detail);
    m_lectureDetailLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
	scrollView->addSubview(m_lectureDetailLabel);
    
    //yHight += m_lectureDetailLabel->getLabelSize().height;
    
    //yHight = yHight + (40);
    

	// tail
	CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
	sView->setFrame(DRect((0), m_winSize.height - (100), m_winSize.width, (100)));
	this->getView()->addSubview(sView);

	m_storeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_collect_pre.png"));
	m_storeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
	m_storeBtnImage->setFrame(DRect((30), (10), (80), (80)));
	CAButton* storeBtn = CAButton::createWithFrame(DRect((0), (0), (125), (100)), CAButtonTypeCustom);
	storeBtn->setAllowsSelected(true);
	//storeBtn->setBackgroundViewForState(CAControlStateAll, m_storeBtnImage);
	storeBtn->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	storeBtn->addSubview(m_storeBtnImage);
	storeBtn->setTag(300);

//	m_canStore = true;
	if (m_msg->m_stored)
	{
		m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
	}
	else
	{
		m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
	}
	sView->addSubview(storeBtn);

	m_likeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_collect_pre.png"));
	m_likeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
	m_likeBtnImage->setFrame(DRect((15), (10), (80), (80)));
	CAButton* likeBtn = CAButton::createWithFrame(DRect((130), (0), (125), (100)), CAButtonTypeCustom);
	likeBtn->setAllowsSelected(true);
	likeBtn->setTag(400);
	//likeBtn->setBackgroundViewForState(CAControlStateAll, m_likeBtnImage);
	likeBtn->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	likeBtn->addSubview(m_likeBtnImage);

//	m_canLike = !(m_msg->m_liked);
	if (m_msg->m_liked)
	{
		m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
	}
	else
	{
		m_likeBtnImage->setImage(CAImage::create("common/btn_like.png"));
	}
	sView->addSubview(likeBtn);

	m_likeNumLabel = CALabel::createWithFrame(DRect((230), (35), (50), (35)));
	m_likeNumLabel->setColor(CAColor_blue);
	m_likeNumLabel->setTextAlignment(CATextAlignmentLeft);
	m_likeNumLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	m_likeNumLabel->setFontSize((25));
    m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
	m_likeNumLabel->setTouchEnabled(false);
	sView->addSubview(m_likeNumLabel);

    /*
	m_surveyButtonView[0] = CAView::createWithFrame(DRect((0), (0), (400), (100)));
	m_surveyButtonView[0]->setColor(CAColor_clear);
    CALabel* label = CALabel::createWithFrame(DRect((0), (0), (400), (100)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	label->setColor(CAColor_white);
	label->setFontSize((35));
	label->setText("Click to win point");
	m_surveyButtonView[0]->addSubview(label);
	m_surveyButtonView[0]->setVisible(false);
*/
	m_surveyButtonView = CAView::createWithFrame(DRect((0), (0), m_winSize.width, (100)));
    m_surveyButtonView->setColor(ccc4(0x99, 0x99, 0x99, 0xff));//CAColor_clear);
    
    if (m_surveyBtnLabel1 == NULL) {
        m_surveyBtnLabel1 = CALabel::createWithFrame(DRect((0), (20), m_winSize.width - (300), (40)));
        m_surveyBtnLabel1->setTextAlignment(CATextAlignmentCenter);
        m_surveyBtnLabel1->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_surveyBtnLabel1->setColor(CAColor_white);
        m_surveyBtnLabel1->setFontSize((30));
        m_surveyBtnLabel1->setText("No available");
        m_surveyButtonView->addSubview(m_surveyBtnLabel1);
    }
    
    if (m_surveyBtnLabel2 == NULL) {
        m_surveyBtnLabel2 = CALabel::createWithFrame(DRect((0), (60), m_winSize.width - (300), (30)));
        m_surveyBtnLabel2->setTextAlignment(CATextAlignmentCenter);
        m_surveyBtnLabel2->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_surveyBtnLabel2->setColor(CAColor_white);
        m_surveyBtnLabel2->setFontSize((20));
        m_surveyBtnLabel2->setText("(Avalible in ?s)");
        m_surveyButtonView->addSubview(m_surveyBtnLabel2);
    }
    
    if (m_surveyBtnLabel3 == NULL) {
        m_surveyBtnLabel3 = CALabel::createWithFrame(DRect((0), (30), m_winSize.width - (300), (35)));
        //m_surveyBtnLabel3 = CALabel::createWithFrame(DRect(m_winSize.width - (300), (0), (300), (100)));
        m_surveyBtnLabel3->setTextAlignment(CATextAlignmentCenter);
        m_surveyBtnLabel3->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_surveyBtnLabel3->setColor(CAColor_white);
        m_surveyBtnLabel3->setFontSize((30));
        m_surveyBtnLabel3->setText("");
        m_surveyBtnLabel3->setVisible(false);
        m_surveyButtonView->addSubview(m_surveyBtnLabel3);
    }

    if (m_surveyBtn == NULL)
    {
        m_surveyBtn = CAButton::createWithFrame(DRect((300), 0, m_winSize.width - (300), (100)), CAButtonTypeCustom);
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/dgray_bg.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        m_surveyBtn->setBackgroundViewForState(CAControlStateAll, imageView);
        imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        m_surveyBtn->setBackgroundViewForState(CAControlStateNormal, imageView);
        m_surveyBtn->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_surveyBtn->setControlState(CAControlStateDisabled);
        m_surveyBtn->setTag(200);
        sView->addSubview(m_surveyBtn);
        
        m_surveyBtn->addSubview(m_surveyButtonView);
    }
}

void SessionDetailViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


void SessionDetailViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
	else if (btn->getTag() == 100)
	{
		this->getView()->removeSubview(p_alertView);
		p_alertView = NULL;
		requestMsg();
	}
    else if (btn->getTag() == 200)
    {
		SurveyViewController* vc = new SurveyViewController(m_msg->m_sessionId);
        vc->init();
        vc->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
	else if (btn->getTag() == 300)
	{
		requestStore();
	}
	else if (btn->getTag() == 400)
	{
		requestLike();
	}
}

void SessionDetailViewController::requestMsg()
{
	std::map<std::string, std::string> key_value;
	key_value["tag"] = sessionDetailViewTag[0];
	key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
	CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionDetailViewController::onRequestFinished));
	{
		p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
		p_pLoading->setLoadingMinTime(0.5f);
	}
}

void string_replace(string&s1, const string&s2, const string&s3)
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}

}
void SessionDetailViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
		
		//CCLog("receive json == %s",tempjson.c_str());
		const CSJson::Value& value = json["result"];

		if (value["r"].asString() == "1")
		{
			const CSJson::Value& v1 = value["s"];
			m_detailMsg.m_sessionId = v1[0]["SessionId"].asInt();
			m_detailMsg.m_detail = v1[0]["Description"].asString();
            m_detailMsg.m_startTime = v1[0]["StartTime"].asInt64();
            m_detailMsg.m_endTime = v1[0]["EndTime"].asInt64();
			const CSJson::Value& v2 = value["sp"];
            m_isSurveyed = value["sv"].asBool();
			for (int i = 0; i < v2.size(); i++)
			{
				speakerMsg speaker;
				speaker.name = crossapp_format_string("%s %s", v2[i]["FirstName"].asCString(), v2[i]["LastName"].asCString());
				speaker.title = v2[i]["Title"].asString();
				speaker.iconUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), v2[i]["Icon"].asCString());
				speaker.role = v2[i]["Role"].asString();
				m_detailMsg.m_speaker.push_back(speaker);
			}
        }
		initView();
	}

#ifdef LOCALTEST
	{
		m_detailMsg.m_sessionId = 12;
		m_detailMsg.m_detail = "asdfasdfasd";
		for (int i = 0; i < 2; i++)
		{
			speakerMsg speaker;
			speaker.name = "Lisa Chen";
			speaker.title = "Custome Success";
			speaker.iconUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
			speaker.role = "Presenter";
			m_detailMsg.m_speaker.push_back(speaker);
		}
		initView();
	}

#endif
	if (p_pLoading)
	{
		p_pLoading->stopAnimating();
	}
}


void SessionDetailViewController::requestStore()
{
//	if (m_canStore)
	{
		std::map<std::string, std::string> key_value;
		key_value["tag"] = sessionViewTag[1];
		key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
		key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
		key_value["v"] = crossapp_format_string("%d", m_msg->m_stored ? 0 : 1);
		//key_value["sign"] = getSign(key_value);
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionDetailViewController::onStoreRequestFinished));

//		m_canStore = false;
	}
}

void SessionDetailViewController::onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        if (value["r"].asBool() == true)
        {
            FNoticeManager::sharedFNoticeManager()->addNotice(m_msg->m_sessionId, notice_session, m_msg->m_title, m_msg->m_title, m_msg->m_startTime, m_msg->m_endTime);
            m_msg->m_stored = true;
            m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
        }
        else
        {
            FNoticeManager::sharedFNoticeManager()->deleteNotice(m_msg->m_sessionId);
            m_msg->m_stored = false;
            m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
        }
        MainViewTableCell::updateTable();
    }
//    m_canStore = true;
}

void SessionDetailViewController::requestLike()
{
//	if (m_canLike)
	{/*
		std::map<std::string, std::string> key_value;
		key_value["tag"] = sessionViewTag[2];
		key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
		key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
		key_value["v"] = crossapp_format_string("%d", 1);
		//key_value["sign"] = getSign(key_value);
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionDetailViewController::onLikeRequestFinished));
      */
        
        std::map<std::string, std::string> key_value;
        key_value["tag"] = sessionViewTag[2];
        key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
        key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
        key_value["v"] = crossapp_format_string("%d", m_msg->m_liked ? 0 : 1);
        //key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionDetailViewController::onLikeRequestFinished));
	}
}

void SessionDetailViewController::onLikeRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
        /*
		m_msg->m_likeNum += 1;
		m_msg->m_liked = true;
		m_canLike = false;
		m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
		m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        */
        const CSJson::Value& value = json["result"];
        if (value["r"].asBool() == true)
        {
            m_msg->m_likeNum += 1;
            m_msg->m_liked = true;
            m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
        }
        else
        {
            m_msg->m_likeNum -= 1;
            m_msg->m_liked = false;
            m_likeBtnImage->setImage(CAImage::create("common/btn_like.png"));
        }
        m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
        MainViewTableCell::updateTable();
	}
    /*
	else
	{
		m_canLike = !(m_msg->m_liked);
	}*/
}

void SessionDetailViewController::showAlert()
{
	if (p_alertView) {
		this->getView()->removeSubview(p_alertView);
		p_alertView = NULL;
	}

	p_alertView = CAView::createWithFrame(DRect((0), (120), m_winSize.width, m_winSize.height - (120)));
	p_alertView->setColor(CAColor_clear);
	this->getView()->addSubview(p_alertView);

	CAButton* button = CAButton::create(CAButtonTypeCustom);
	button->setTag(100);
	button->setColor(CAColor_clear);
	button->setFrame(DRect(0, 0, m_winSize.width, m_winSize.height - (120)));
	button->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	p_alertView->addSubview(button);

	CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - (300), m_winSize.width, (30)));
	test->setColor(CAColor_gray);
	test->setTextAlignment(CATextAlignmentCenter);
	test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	test->setFontSize((24));
	test->setText("Network cannot connect!");
	test->setTouchEnabled(false);
	p_alertView->addSubview(test);

}

void SessionDetailViewController::adjustSurveyBtn(float dt)
{
    time_t nowTime = getTimeSecond();
//    CCLog("start : %d", m_detailMsg.m_startTime);
//    CCLog("end   : %d", m_detailMsg.m_endTime);

    if (m_isSurveyed)
    {// is attended this survey
        if (m_surveyBtn) m_surveyBtn->setControlState(CAControlStateDisabled);
        if (m_surveyBtnLabel1) {
            m_surveyBtnLabel1->setVisible(false);
        }
        if (m_surveyBtnLabel2) {
            m_surveyBtnLabel2->setVisible(false);
        }
        if (m_surveyBtnLabel3) {
            m_surveyBtnLabel3->setText("Attended");
            m_surveyBtnLabel3->setVisible(true);
        }
    } else {
        if (m_detailMsg.m_endTime == 0 || m_detailMsg.m_startTime == 0) {
            // not load yet
            if (m_surveyBtn) m_surveyBtn->setControlState(CAControlStateDisabled);
            if (m_surveyBtnLabel1) {
                m_surveyBtnLabel1->setText("null");
            }
            if (m_surveyBtnLabel2) m_surveyBtnLabel2->setColor(CAColor_blue);
        } else if (nowTime > m_detailMsg.m_endTime + TIME_GAPE) {
            // close
            if (m_surveyBtn) m_surveyBtn->setControlState(CAControlStateDisabled);
            if (m_surveyBtnLabel1) {
                m_surveyBtnLabel1->setVisible(false);
            }
            if (m_surveyBtnLabel2) {
                m_surveyBtnLabel2->setVisible(false);
            }
            if (m_surveyBtnLabel3) {
                m_surveyBtnLabel3->setText("Overdue");
                m_surveyBtnLabel3->setVisible(true);
            }
        } else if (nowTime < m_detailMsg.m_startTime + TIME_GAPE) {
            // not start yet
            if (m_surveyBtn) m_surveyBtn->setControlState(CAControlStateDisabled);
            if (m_surveyBtnLabel1) {
                m_surveyBtnLabel1->setText("Click to win credits");
//                m_surveyBtnLabel1->setColor(CAColor_blue);
            }
            if (m_surveyBtnLabel2) {
                time_t delteTm = m_detailMsg.m_startTime + TIME_GAPE - nowTime;
                struct tm * timeinfo;
                timeinfo = gmtime(&delteTm);
                string timeStr = "";
                if (timeinfo->tm_yday != 0) {
                    timeStr += crossapp_format_string("%dd",timeinfo->tm_yday);
                }
                if (timeinfo->tm_hour != 0) {
                    timeStr += crossapp_format_string("%dh",timeinfo->tm_hour);
                }
                if (timeinfo->tm_min != 0) {
                    timeStr += crossapp_format_string("%dm",timeinfo->tm_min);
                }
                if (timeinfo->tm_sec != 0) {
                    timeStr += crossapp_format_string("%ds",timeinfo->tm_sec);
                }
                m_surveyBtnLabel2->setText(crossapp_format_string("Avaliable is %s", timeStr.c_str()));
//                m_surveyBtnLabel2->setColor(SAP_DEFAULT_COLOR);
            }
        } else {
            // can attend
            if (m_surveyButtonView) {
                m_surveyButtonView->setColor(SAP_DEFAULT_COLOR);
            }
            if (m_surveyBtn) {
                m_surveyBtn->setControlState(CAControlStateNormal);
            }
            if (m_surveyBtnLabel1) {
                m_surveyBtnLabel1->setVisible(false);
            }
            if (m_surveyBtnLabel2) {
                m_surveyBtnLabel2->setVisible(false);
            }
            if (m_surveyBtnLabel3) {
                m_surveyBtnLabel3->setText("Click to win credits");
                m_surveyBtnLabel3->setVisible(true);
            }
        }
    }
}