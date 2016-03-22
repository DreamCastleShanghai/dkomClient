
#ifndef __Main__ViewController__
#define __Main__ViewController__

#include "RootWindow.h"

class MainViewController :
public CAViewController,
public CATableViewDelegate,
public CATableViewDataSource,
public CAPageViewDelegate,
public CACollectionViewDelegate,
public CAScrollViewDelegate,
public CADatePickerViewDelegate
{
    
public:

	MainViewController();
    
	virtual ~MainViewController();
    
public:
    
    void onNotice();
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    void update(float dt);
    
//	void initMsgTableView();

//	void initPageView();
    
    void requestSessionMsg();
    
    void initPageView();
    
    bool checkPageStatusIsChanged();
    
    void adjustPageViewContent();
    
//    void requestIconMsg();

	void showAlert();

	void refreshBtnCallBack(CAControl* btn, DPoint point);

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

	//void onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

	void buttonCallBack(CAControl* btn, DPoint point);
    
    void updatePageView(float dt);
    
    void refreshTable();
    
    void reloadTableData();
    
    void goSustainabilityCampaign();
    
    void goDKomSurvey();
    
    void goDemoJam();
    
    void goUploadPhoto();
    
    void goCreditPointRules();

	virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
	virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);

    CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section);
	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual void pageViewDidBeginTurning(CAPageView* pageView);
	virtual void pageViewDidEndTurning(CAPageView* pageView);
    virtual void pageViewDidSelectedPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
    virtual void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
    
	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    
    void refreshNoticeRedPoint();
private:
    enum
    {
        MAP_ALL_MAP,
        MAP_PRE_EVENT,
        MAP_MORNING,
        MAP_AFTERNOON,
        MAP_EVENING,
        
        MAP_MAX,
    };
    // page view relative member
    int                                                     m_pageStatus;
    std::vector<newsPage>									m_page;
    std::vector<newsPage>                                   m_pageAllMapVec;
    CAPageView*												m_pageView;
    CAPageControl*											m_pageControl;
    CAView*                                                 m_pageControlBG;
    //	CALabel*												m_pageViewTitle;
    
	std::vector<sessionMsg>*								m_msg;
    std::vector<sessionMsg*>								m_filterMsg;

    CAImageView*                                            m_timeNoticeImageView;
    CAImageView*                                            m_sessionNoticeImageView;
//    CAPageView*                                             m_totalView;
//    CAVector<CAView*>                                       m_totalViewList;
	CATableView*											m_msgTableView;
	CAView*													p_alertView;
	CAActivityIndicatorView*								p_pLoading;
//    int														m_pastSection;
//	int														m_nextSection;
    time_t                                                  m_timeForPageView;

    DSize													m_winSize;
    CAViewController*                                       m_sustainabilitySurvey;
    CAViewController*                                       m_monent;
    CAViewController*                                       m_vote;
    CAViewController*                                       m_map;
    CAViewController*                                       m_session;
    CAViewController*                                       m_my;
    
    CAPullToRefreshView *                               m_headRefreshView;
    CAPullToRefreshView *                               m_rootRefreshView;
};


#endif /* defined(__Login__ViewController__) */
