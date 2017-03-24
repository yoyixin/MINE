//
// Author  : Sai
// Created : Oct 2012
//
// extra_ticket_master.cpp -
//

#include <string>
#include <iostream>
#include <fstream>
#include <omronsoft/log.h>
#include <omronsoft/sa1/pub/svc_util.h>
#include <omronsoft/sa1/info/erroccur.h>
#include <omronsoft/sa1/pub/double_data.h>

#include "../master.h"
#include "../master_def.h"
#include "extra_ticket_master_fmt.h"
#include "extra_ticket_master.h"
#include "../../constval/constval.h"
#include "../../../info/errorinfo.h"

using namespace std;
using namespace omronsoft;
using namespace omronsoft::svc_util;
using namespace omronsoft::sa1;
using namespace omronsoft::pub::master;
using namespace omronsoft::pub::extraticketmaster;
using namespace omronsoft::pub::doubledata;
using namespace omronsoft::pub::constval;

//異常を投げる
#define THROW_ERROR(A,B,C,D) \
	omronsoft::exception::ErrorCode ec; \
	ec.code = MasterDef::ERROR_ID; \
	ec.detail = A; \
	throw omronsoft::exception::Error(ec, B, C, D);

// ↓[(H11.0) （４） ｃ．] Start ADD
#define NOTICE_DBLDATA_ERROR(CD,X,Y) \
{std::string msg(X); \
msg += Y; \
ErrCode2mtc ec((int)MasterDef::ERROR_ID, CD); \
int mtc = ec.mtc(); \
OMRONSOFT_TRACE("NOTICE_DBLDATA_ERROR Error Occur. mtc = " << mtc << "  " << msg.c_str()); \
INTERNAL_ERROR(0, mtc, msg.c_str());}

#define COUNT_DBLDATA_ERROR(CD,X,Y) \
{std::string msg(X); \
msg += Y; \
ErrCode2mtc ec((int)MasterDef::ERROR_ID, CD); \
int mtc = ec.mtc(); \
OMRONSOFT_TRACE("COUNT_DBLDATA_ERROR Error count. mtc = " << mtc << "  " << msg.c_str()); \
MultiProps errmp; \
errmp.set_value(ErrOccur::EC,      0);\
errmp.set_value(ErrOccur::MTC,     mtc);\
errmp.set_value(ErrOccur::RECOVER, ErrOccur::CU);\
errmp.set_value(ErrOccur::WORDING, static_cast<char*>(0));\
errmp.set_value(ErrOccur::MODULE,  static_cast<const char*>(__FILE__));\
errmp.set_value(ErrOccur::LINENO,  static_cast<int>(__LINE__));\
errmp.set_value(ErrOccur::REMERK,  msg.c_str());\
omronsoft::kernel::Repository::instance().update(ErrCount::NAME, errmp);}
// ↑[(H11.0) （４） ｃ．] End   ADD


namespace omronsoft {
namespace pub {
namespace extraticketmaster {
class ExtraTicketAccountRep{
	friend class ExtraTicketAccount;
	friend class EachExtraTicketMasterRep;


	//企画券No
	unsigned short extra_ticket_no() const { return td_.extra_ticket_no_; }

	//発売データ種別No
	unsigned short issue_data_kind_no() const { return td_.issue_data_kind_no_; }

	//発売データNo
	unsigned short issue_data_no() const { return td_.issue_data_no_; }

	//払戻データ種別No
	unsigned short refund_data_kind_no() const { return td_.refund_data_kind_no_; }

	//払戻データNo
	unsigned short refund_data_no() const { return td_.refund_data_no_; }

	//発売金額（大人）
	unsigned short issue_amount_adult() const { return td_.issue_amount_adult_; }

	//発売金額（小児）
	unsigned short issue_amount_child() const { return td_.issue_amount_child_; }

	//n枚目表示運賃（大人）
	unsigned short ticket_fare_adult(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			unsigned short ticket_fare = 0;
			switch (count) {
				case 1:ticket_fare = td_.ticketface1_fare_adult_;break;
				case 2:ticket_fare = td_.ticketface2_fare_adult_;break;
				case 3:ticket_fare = td_.ticketface3_fare_adult_;break;
				case 4:ticket_fare = td_.ticketface4_fare_adult_;break;
				default:break;
			}
			return ticket_fare; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//n枚目表示運賃（小児）
	unsigned short ticket_fare_child(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			unsigned short ticket_fare = 0;
			switch (count) {
				case 1:ticket_fare = td_.ticketface1_fare_child_;break;
				case 2:ticket_fare = td_.ticketface2_fare_child_;break;
				case 3:ticket_fare = td_.ticketface3_fare_child_;break;
				case 4:ticket_fare = td_.ticketface4_fare_child_;break;
				default:break;
			}
			return ticket_fare; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//発行枚数
	unsigned short issue_num() const { return td_.issue_num_; }

	//発売開始年月日
	Timestamp issue_start_date() const { return td_.issue_start_date_; }

	//発売終了年月日
	Timestamp issue_end_date() const { return td_.issue_end_date_; }

	//無期限であるかどうか
	bool is_indefinite() const { return td_.is_indefinite_; }

	//前売開始年月日
	Timestamp pre_issue_start_date() const { return td_.pre_issue_start_date_; }

	//前売があるかどうか
	bool is_pre_issue() const { return td_.is_pre_issue_; }

//	//多機能券売機用有効日数
//	unsigned short v_valid_date_count() const { return td_.v_valid_date_count_; }

	//窓処用有効日数有効日数
	unsigned short cm_valid_date_count() const { return td_.cm_valid_date_count_; }

	//イベント固定有効開始日
	Timestamp event_start_date() const { return td_.event_start_date_; }

	//イベント固定有効終了日
	Timestamp event_end_date() const { return td_.event_end_date_; }

//	//多機能券売機発売可否 発売可否OFFの場合、発売、払戻ともに不可
//	bool v_issue_permit() const { return td_.v_issue_permit_; }
//
//	//多機能券売機払戻可否 発売可否OFFの場合、発売、払戻ともに不可 かつエンコード券枚数は１枚のみ
//	bool v_refund_permit() const { return td_.v_refund_permit_ && td_.v_issue_permit_ && tfd_.encode_num_ == 1; }

	//窓口処理機発売可否 かつエンコード券枚数は１枚のみ
	bool cm_issue_permit() const { return td_.cm_issue_permit_; }

	//窓口処理機払戻可否
	bool cm_refund_permit() const { return td_.cm_refund_permit_ && tfd_.encode_num_ == 1; }

	//土休日
	bool is_holiday() const { return td_.is_holiday_; }

	//企画券制限設定（平日）開始時
	unsigned short limit_weekday_set_start_hour() const { return td_.limit_weekday_set_start_hour_; }

	//企画券制限設定（平日）開始分
	unsigned short limit_weekday_set_start_minute() const { return td_.limit_weekday_set_start_minute_; }

	//企画券制限設定（平日）終了時
	unsigned short limit_weekday_set_end_hour() const { return td_.limit_weekday_set_end_hour_; }

	//企画券制限設定（平日）終了分
	unsigned short limit_weekday_set_end_minute() const { return td_.limit_weekday_set_end_minute_; }

	//企画券制限設定（休日）開始時
	unsigned short limit_holiday_set_start_hour() const { return td_.limit_holiday_set_start_hour_; }

	//企画券制限設定（休日）開始分
	unsigned short limit_holiday_set_start_minute() const { return td_.limit_holiday_set_start_minute_; }

	//企画券制限設定（休日）終了時
	unsigned short limit_holiday_set_end_hour() const { return td_.limit_holiday_set_end_hour_; }

	//企画券制限設定（休日）終了分
	unsigned short limit_holiday_set_end_minute() const { return td_.limit_holiday_set_end_minute_; }
//
//	//発売可能チェック（券売機用） true:発売可、false:発売不可
//	// INPUT:Timestamp date実日付、int days平日か土休日か、preはtrue場合は事前確認　時刻制限判断せず
//    bool sale_judge_v(Timestamp date, int days, bool pre) const {
//		bool ret = false;
//		for(;;) {
//			// 多機能券売機発売可否判断
//			if(!td_.v_issue_permit_) break;
//			// 発売期間チェック 
//			// 発売開始日は運用日で比較
//			OpDateTime datetime(date);
//			// 定期券運用日付　3:00
//            Timestamp date_op = datetime.season_system();
//			if(date_op < td_.issue_start_date_) break;
//			// 発売終了日は実日付で比較
//			Date date_true(date);
//			Date end_date_true(td_.issue_end_date_);
//			if(!td_.is_indefinite_ && date_true > end_date_true) break;
//
//			// 土休日判断
//			// 券売機有効日数は当日のみ
//			if(td_.is_holiday_ && days != ExtraTicketAccount::HOLIDAY) break;
//			// preはtrue場合は事前確認　時刻制限判断せず
//			if(!pre) {
//				// 企画券時刻制限判定（休日）
//				int s_time = 0;
//				int e_time = 0;
//				int c_time = static_cast<int>(date.hour()*10000 + date.minute()*100 + date.second());
//				if(days == ExtraTicketAccount::HOLIDAY) {
//					s_time = td_.limit_holiday_set_start_hour_*10000 + td_.limit_holiday_set_start_minute_*100;
//					e_time = td_.limit_holiday_set_end_hour_*10000   + td_.limit_holiday_set_end_minute_*100;
//					// 企画券時刻制限判定（平日）
//				} else if (days == ExtraTicketAccount::WEEKDAY) {
//					s_time = td_.limit_weekday_set_start_hour_*10000 + td_.limit_weekday_set_start_minute_*100;
//					e_time = td_.limit_weekday_set_end_hour_*10000   + td_.limit_weekday_set_end_minute_*100;
//				} else { 
//					THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"days error!!",__FILE__,__LINE__); 
//				}
//				OMRONSOFT_TRACE("区間制限開始時刻 = " << s_time << "  区間制限終了時刻 = " << e_time << "  現在時刻 = " << c_time)
//					int vres = e_time - s_time; if(vres < 0) vres += 240000;
//				int vcur = c_time - s_time; if(vcur < 0) vcur += 240000;
//				if(vres > vcur) break;                                               // 区間制限時間内
//			}
//			ret = true;
//			break;
//		}
//		return ret;
//	}
//
	//発売可能チェック（窓処用） true:発売可、false:発売不可
	// INPUT:Timestamp date実日付、int days平日か土休日か、preはtrue場合は事前確認　時刻制限判断せず
    bool sale_judge_cm(Timestamp date, int days, bool pre) const {
		bool ret = false;
		for(;;) {
			// 窓処発売可否判断
			if(!td_.cm_issue_permit_) break;
			// 前売りあり
			if(td_.is_pre_issue_) {
				// 前売りチェック
				// 発売開始日　≧　前売り開始日
				// 前売日は運用日で比較
				// 定期券運用日付　3:00
				OpDateTime datetime_pre(date);
				Timestamp date_op_pre = datetime_pre.season_system();
				if(date_op_pre < td_.pre_issue_start_date_) break;
			} else {
				// 発売期間チェック 
				// 発売開始日は運用日で比較
				// 定期券運用日付　3:00
				OpDateTime datetime(date);
				Timestamp date_op = datetime.season_system();
				if(date_op < td_.issue_start_date_) break;
			}
			// 発売終了日は実日付で比較
			Date date_true(date);
			Date end_date_true(td_.issue_end_date_);
			if(!td_.is_indefinite_ && date_true > end_date_true) break;

			// 有効日数が１の場合、および前売り券ではない　土休日判断
			//if(td_.cm_valid_date_count_ == 1 && !td_.is_pre_issue_ && !pre) {
			if(!pre) {
				if(td_.is_holiday_ && days != ExtraTicketAccount::HOLIDAY) break;
			}

			// preはtrue場合は事前確認　時刻制限判断せず
			if(!pre) {
				// 企画券時刻制限判定（休日）
				int s_time = 0;
				int e_time = 0;
				int c_time = static_cast<int>(date.hour()*10000 + date.minute()*100 + date.second());
				if(days == ExtraTicketAccount::HOLIDAY) {
					s_time = td_.limit_holiday_set_start_hour_*10000 + td_.limit_holiday_set_start_minute_*100;
					e_time = td_.limit_holiday_set_end_hour_*10000   + td_.limit_holiday_set_end_minute_*100;
					// 企画券時刻制限判定（平日）
				} else if (days == ExtraTicketAccount::WEEKDAY) {
					s_time = td_.limit_weekday_set_start_hour_*10000 + td_.limit_weekday_set_start_minute_*100;
					e_time = td_.limit_weekday_set_end_hour_*10000   + td_.limit_weekday_set_end_minute_*100;
				} else { 
					THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"days error!!",__FILE__,__LINE__); 
				}
				OMRONSOFT_TRACE("区間制限開始時刻 = " << s_time << "  区間制限終了時刻 = " << e_time << "  現在時刻 = " << c_time)
					int vres = e_time - s_time; if(vres < 0) vres += 240000;
				int vcur = c_time - s_time; if(vcur < 0) vcur += 240000;
				if(vres > vcur) break;                                               // 区間制限時間内
			}
			ret = true;
			break;
		}
		return ret;
	}

    //企画券名称
    const char* extra_ticket_name() const { return tfd_.extra_ticket_name_.c_str(); }

	//口座ボタン券売機和文1段目
	const char* v_button_word_jp_1() const { return tfd_.v_button_word_jp_1_.c_str(); }

	//口座ボタン券売機和文2段目
	const char* v_button_word_jp_2() const { return tfd_.v_button_word_jp_2_.c_str(); }

	//口座ボタン券売機英文1段目
	const char* v_button_word_en_1() const { return tfd_.v_button_word_en_1_.c_str(); }

	//口座ボタン券売機英文2段目
	const char* v_button_word_en_2() const { return tfd_.v_button_word_en_2_.c_str(); }

	//口座ボタン窓処和文1段目
	const char* cm_button_word_jp_1() const { return tfd_.cm_button_word_jp_1_.c_str(); }

	//口座ボタン窓処和文2段目
	const char* cm_button_word_jp_2() const { return tfd_.cm_button_word_jp_2_.c_str(); }


	//券種（n枚目）
	unsigned short ticket_kind(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].ticket_kind; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//券面ファイル名前（n枚目）
	const char* ticketface_name(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].ticketface_name.c_str(); }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//金額印字有無（n枚目）
	bool is_amount_print(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].is_amount_print; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//発駅（n枚目）
	void start_station(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].start_station.line;
			station = tfd_.issue_info_[count - 1].start_station.station; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//ナンバリング印字有無（n枚目）
	bool is_numbering_print(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].is_numbering_print; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//口座ボタン表示順序
	unsigned short button_index() const { return td_.button_index_; }

	//ベース券面コード（Ａ～Ｒ）で判断する割引有無（n枚目）
	bool discount_by_format_code(unsigned short count) const {
		bool ret = false;
		std::string format_code = "";
		format_code = tfd_.issue_info_[count - 1].format_code;
		if( (format_code == string("Ｂ")) ||
			(format_code == string("Ｄ")) ||
			(format_code == string("Ｆ")) ||
			(format_code == string("Ｈ")) ||
			(format_code == string("Ｊ")) ||
			(format_code == string("Ｌ")) ){
				ret = true;
		}
		return ret;
	}

	//------------大型券エンコード用------------
	//券種３（大型券）（n枚目）
	unsigned short fmbigst_ticket_kind3(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_ticket_kind3; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//発駅１（大型券）（n枚目）
	void fmbigst_start_station1(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_start_station1.line;
			station = tfd_.issue_info_[count - 1].fmbigst_start_station1.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//発駅２（大型券）（n枚目）
	void fmbigst_start_station2(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_start_station2.line;
			station = tfd_.issue_info_[count - 1].fmbigst_start_station2.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//発駅３（大型券）（n枚目）
	void fmbigst_start_station3(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_start_station3.line;
			station = tfd_.issue_info_[count - 1].fmbigst_start_station3.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//着駅１（大型券）（n枚目）
	void fmbigst_arrive_station1(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_arrive_station1.line;
			station = tfd_.issue_info_[count - 1].fmbigst_arrive_station1.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//着駅２（大型券）（n枚目）
	void fmbigst_arrive_station2(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_arrive_station2.line;
			station = tfd_.issue_info_[count - 1].fmbigst_arrive_station2.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//着駅３（大型券）（n枚目）
	void fmbigst_arrive_station3(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_arrive_station3.line;
			station = tfd_.issue_info_[count - 1].fmbigst_arrive_station3.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由１（大型券）（n枚目）
	void fmbigst_via_station1(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station1.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station1.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由２（大型券）（n枚目）
	void fmbigst_via_station2(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station2.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station2.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由３（大型券）（n枚目）
	void fmbigst_via_station3(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station3.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station3.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由４（大型券）（n枚目）
	void fmbigst_via_station4(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station4.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station4.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由５（大型券）（n枚目）
	void fmbigst_via_station5(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station5.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station5.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由６（大型券）（n枚目）
	void fmbigst_via_station6(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station6.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station6.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由７（大型券）（n枚目）
	void fmbigst_via_station7(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station7.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station7.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由８（大型券）（n枚目）
	void fmbigst_via_station8(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station8.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station8.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由９（大型券）（n枚目）
	void fmbigst_via_station9(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station9.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station9.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由１０（大型券）（n枚目）
	void fmbigst_via_station10(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station10.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station10.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//フリー区間コード（大型券）（n枚目）
	void fmbigst_free_area_code(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_free_area_code.line;
			station = tfd_.issue_info_[count - 1].fmbigst_free_area_code.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//券種４詳細（大型券）（n枚目）
	unsigned short fmbigst_ticket_kind4_detail(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_ticket_kind4_detail; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//往復情報（大型券）（n枚目）
	unsigned char fmbigst_roundtrip_info(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_roundtrip_info; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//時差・土休（大型券）（n枚目）
	unsigned char fmbigst_ticket_type(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_ticket_type; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//商品コード（大型券）（n枚目）
	unsigned long fmbigst_item_code(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_item_code; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//割引コード（大型券）（n枚目）
	unsigned char fmbigst_discount_code(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_discount_code; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//------------定期券エンコード用------------
	//券種３（定期券）（n枚目）
	unsigned short fmpass_ticket_kind3(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmpass_ticket_kind3; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//発駅（定期券）（n枚目）
	void fmpass_start_station(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_start_station.line;
			station = tfd_.issue_info_[count - 1].fmpass_start_station.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//着駅（定期券）（n枚目）
	void fmpass_arrive_station(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_arrive_station.line;
			station = tfd_.issue_info_[count - 1].fmpass_arrive_station.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由１（定期券）（n枚目）
	void fmpass_via_station1(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station1.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station1.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由２（定期券）（n枚目）
	void fmpass_via_station2(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station2.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station2.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由３（定期券）（n枚目）
	void fmpass_via_station3(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station3.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station3.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由４（定期券）（n枚目）
	void fmpass_via_station4(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station4.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station4.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由５（定期券）（n枚目）
	void fmpass_via_station5(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station5.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station5.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由６（定期券）（n枚目）
	void fmpass_via_station6(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station6.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station6.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由７（定期券）（n枚目）
	void fmpass_via_station7(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station7.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station7.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//経由８（定期券）（n枚目）
	void fmpass_via_station8(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station8.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station8.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//２重化（定期券）（n枚目）
	bool fmpass_doubleness_bit(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmpass_doubleness_bit; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}


	ExtraTicketAccountRep(): cnt_(1){}
	virtual ~ExtraTicketAccountRep(){ cnt_ = -1; }
	ExtraTicketAccountRep* dup() {
		ExtraTicketAccountRep* ret = 0;
		if (cnt_ > 0) {
			++cnt_;
			ret = this;
		}
		return ret;
	}

	void rel() {
		if (--cnt_ == 0) {
			delete this;
		}
	}

	TicketData td_;
	TicketFormatData tfd_;
	int cnt_;
};

ExtraTicketAccount::ExtraTicketAccount() : rep_(new ExtraTicketAccountRep()) {}

ExtraTicketAccount::ExtraTicketAccount(const ExtraTicketAccount& c) : rep_(c.rep_) {
	if (rep_) {
		rep_->dup();
	}
}

ExtraTicketAccount& ExtraTicketAccount::operator=(const ExtraTicketAccount& c) {
	if (this != &c) {
		if (rep_) {
			rep_->rel();
			rep_ = 0;
		}
		if (c.rep_) {
			rep_ = c.rep_->dup();
		}
	}
	return *this;
}

ExtraTicketAccount::~ExtraTicketAccount() {
	if (rep_) {
		rep_->rel();
		rep_ = 0;
	}
}

unsigned short ExtraTicketAccount::extra_ticket_no() const {
	if(rep_){ return rep_->extra_ticket_no(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::issue_data_kind_no() const {
	if(rep_){ return rep_->issue_data_kind_no(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::issue_data_no() const {
	if(rep_){ return rep_->issue_data_no(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::refund_data_kind_no() const {
	if(rep_){ return rep_->refund_data_kind_no(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::refund_data_no() const {
	if(rep_){ return rep_->refund_data_no(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::issue_amount_adult() const {
	if(rep_){ return rep_->issue_amount_adult(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::issue_amount_child() const {
	if(rep_){ return rep_->issue_amount_child(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::ticket_fare_adult(unsigned short count) const {
	if(rep_){ return rep_->ticket_fare_adult(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::ticket_fare_child(unsigned short count) const {
	if(rep_){ return rep_->ticket_fare_child(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::issue_num() const {
	if(rep_){ return rep_->issue_num(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

Timestamp ExtraTicketAccount::issue_start_date() const {
	if(rep_){ return rep_->issue_start_date(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

Timestamp ExtraTicketAccount::issue_end_date() const {
	if(rep_){ return rep_->issue_end_date(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool ExtraTicketAccount::is_indefinite() const {
	if(rep_){ return rep_->is_indefinite(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

Timestamp ExtraTicketAccount::pre_issue_start_date() const {
	if(rep_){ return rep_->pre_issue_start_date(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool ExtraTicketAccount::is_pre_issue() const {
	if(rep_){ return rep_->is_pre_issue(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

//unsigned short ExtraTicketAccount::v_valid_date_count() const {
//	if(rep_){ return rep_->v_valid_date_count(); }
//	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
//}

unsigned short ExtraTicketAccount::cm_valid_date_count() const {
	if(rep_){ return rep_->cm_valid_date_count(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

Timestamp ExtraTicketAccount::event_start_date() const {
	if(rep_){ return rep_->event_start_date(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

Timestamp ExtraTicketAccount::event_end_date() const {
	if(rep_){ return rep_->event_end_date(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

//bool ExtraTicketAccount::v_issue_permit() const {
//	if(rep_){ return rep_->v_issue_permit(); }
//	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
//}

//bool ExtraTicketAccount::v_refund_permit() const {
//	if(rep_){ return rep_->v_refund_permit(); }
//	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
//}

bool ExtraTicketAccount::cm_issue_permit() const {
	if(rep_){ return rep_->cm_issue_permit(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool ExtraTicketAccount::cm_refund_permit() const {
	if(rep_){ return rep_->cm_refund_permit(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool ExtraTicketAccount::is_holiday() const {
	if(rep_){ return rep_->is_holiday(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::limit_weekday_set_start_hour() const {
	if(rep_){ return rep_->limit_weekday_set_start_hour(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::limit_weekday_set_start_minute() const {
	if(rep_){ return rep_->limit_weekday_set_start_minute(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::limit_weekday_set_end_hour() const {
	if(rep_){ return rep_->limit_weekday_set_end_hour(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::limit_weekday_set_end_minute() const {
	if(rep_){ return rep_->limit_weekday_set_end_minute(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::limit_holiday_set_start_hour() const {
	if(rep_){ return rep_->limit_holiday_set_start_hour(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::limit_holiday_set_start_minute() const {
	if(rep_){ return rep_->limit_holiday_set_start_minute(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::limit_holiday_set_end_hour() const {
	if(rep_){ return rep_->limit_holiday_set_end_hour(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::limit_holiday_set_end_minute() const {
	if(rep_){ return rep_->limit_holiday_set_end_minute(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

//bool ExtraTicketAccount::sale_judge_v(Timestamp date, int days, bool pre) const {
//	if(rep_){ return rep_->sale_judge_v(date, days, pre); }
//	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
//}

bool ExtraTicketAccount::sale_judge_cm(Timestamp date, int days, bool pre) const {
	if(rep_){ return rep_->sale_judge_cm(date, days, pre); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

const char* ExtraTicketAccount::extra_ticket_name() const{
	if(rep_){ return rep_->extra_ticket_name(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

const char* ExtraTicketAccount::v_button_word_jp_1() const{
	if(rep_){ return rep_->v_button_word_jp_1(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

const char* ExtraTicketAccount::v_button_word_jp_2() const{
	if(rep_){ return rep_->v_button_word_jp_2(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

const char* ExtraTicketAccount::v_button_word_en_1() const{
	if(rep_){ return rep_->v_button_word_en_1(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

const char* ExtraTicketAccount::v_button_word_en_2() const{
	if(rep_){ return rep_->v_button_word_en_2(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

const char* ExtraTicketAccount::cm_button_word_jp_1() const{
	if(rep_){ return rep_->cm_button_word_jp_1(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

const char* ExtraTicketAccount::cm_button_word_jp_2() const{
	if(rep_){ return rep_->cm_button_word_jp_2(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::ticket_kind(unsigned short count) const {
	if(rep_){ return rep_->ticket_kind(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

const char* ExtraTicketAccount::ticketface_name(unsigned short count) const {
	if(rep_){ return rep_->ticketface_name(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool ExtraTicketAccount::is_amount_print(unsigned short count) const {
	if(rep_){ return rep_->is_amount_print(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::start_station(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->start_station(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool ExtraTicketAccount::is_numbering_print(unsigned short count) const {
	if(rep_){ return rep_->is_numbering_print(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::button_index() const {
	if(rep_){ return rep_->button_index(); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool ExtraTicketAccount::discount_by_format_code(unsigned short count) const {
	if(rep_){ return rep_->discount_by_format_code(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}
unsigned short ExtraTicketAccount::fmbigst_ticket_kind3(unsigned short count) const {
	if(rep_){ return rep_->fmbigst_ticket_kind3(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_start_station1(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_start_station1(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_start_station2(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_start_station2(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_start_station3(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_start_station3(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_arrive_station1(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_arrive_station1(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_arrive_station2(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_arrive_station2(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_arrive_station3(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_arrive_station3(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_via_station1(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_via_station1(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_via_station2(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_via_station2(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_via_station3(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_via_station3(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_via_station4(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_via_station4(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_via_station5(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_via_station5(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_via_station6(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_via_station6(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_via_station7(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_via_station7(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_via_station8(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_via_station8(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_via_station9(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_via_station9(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_via_station10(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmbigst_via_station10(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmbigst_free_area_code(unsigned short count, unsigned char& line, unsigned char& station) const {
	if(rep_){ rep_->fmbigst_free_area_code(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::fmbigst_ticket_kind4_detail(unsigned short count) const {
	if(rep_){ return rep_->fmbigst_ticket_kind4_detail(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned char ExtraTicketAccount::fmbigst_roundtrip_info(unsigned short count) const {
	if(rep_){ return rep_->fmbigst_roundtrip_info(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned char ExtraTicketAccount::fmbigst_ticket_type(unsigned short count) const {
	if(rep_){ return rep_->fmbigst_ticket_type(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned long ExtraTicketAccount::fmbigst_item_code(unsigned short count) const {
	if(rep_){ return rep_->fmbigst_item_code(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned char ExtraTicketAccount::fmbigst_discount_code(unsigned short count) const {
  if(rep_){ return rep_->fmbigst_discount_code(count); }
  else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

unsigned short ExtraTicketAccount::fmpass_ticket_kind3(unsigned short count) const {
	if(rep_){ return rep_->fmpass_ticket_kind3(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmpass_start_station(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmpass_start_station(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmpass_arrive_station(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmpass_arrive_station(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmpass_via_station1(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmpass_via_station1(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmpass_via_station2(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmpass_via_station2(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmpass_via_station3(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmpass_via_station3(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmpass_via_station4(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmpass_via_station4(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmpass_via_station5(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmpass_via_station5(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmpass_via_station6(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmpass_via_station6(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmpass_via_station7(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmpass_via_station7(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void ExtraTicketAccount::fmpass_via_station8(unsigned short count, unsigned char& line, unsigned char& station) const{
	if(rep_){ rep_->fmpass_via_station8(count, line, station); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool ExtraTicketAccount::fmpass_doubleness_bit(unsigned short count) const {
	if(rep_){ return rep_->fmpass_doubleness_bit(count); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}



class EachExtraTicketMasterRep{
	friend class EachExtraTicketMaster;
	friend class ExtraTicketMasterRep;


	////券売機発売できる口座数取得
	//unsigned short account_number_v(Timestamp date, int days, bool collaboration, bool pre) const{
	//	unsigned short account_n = 0;
	//	for(int k = 0; k < ACCOUNT_NUM; ++k){
	//		unsigned short extra_ticket_no = 0;
	//		if(collaboration) {
	//		  extra_ticket_no = ecd_.button_display_order_sp_[k];
	//		} else {
	//		  extra_ticket_no = ecd_.button_display_order_[k];
	//		}
	//		// 企画券マスタ中に検索
	//		for(int i = 0; i < ecd_.master_record_count_; ++i) {
	//			if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_) {
	//				// 企画券フォーマットデータ中に検索
	//				for(int j = 0; j < eetd_.format_.ticketformat_data_.size(); ++j) {
	//					if(extra_ticket_no == eetd_.format_.ticketformat_data_[j].extra_ticket_no_) {
	//						ExtraTicketAccount eta;
	//						eta.rep_->td_ = eetd_.master_.ticket_data_[i];
	//						eta.rep_->tfd_ = eetd_.format_.ticketformat_data_[j];
	//						if(eta.sale_judge_v(date, days, pre)) { ++account_n;}
	//						break;
	//					}
	//				}
	//				break;
	//			}
	//		}
	//	}
	//	return account_n;
	//}

	//窓処発売できる口座数取得
	unsigned short account_number_cm(Timestamp date, int days, bool collaboration, bool pre) const{
		unsigned short account_n = 0;
		for(int k = 0; k < ACCOUNT_NUM; ++k){
			unsigned short extra_ticket_no = 0;
			if(collaboration) {
			  extra_ticket_no = ecd_.button_display_order_sp_[k];
			} else {
			  extra_ticket_no = ecd_.button_display_order_[k];
			}
			// 企画券マスタ中に検索
			for(int i = 0; i < ecd_.master_record_count_; ++i) {
				if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_) {
					// 企画券フォーマットデータ中に検索
					for(int j = 0; j < eetd_.format_.ticketformat_data_.size(); ++j) {
						if(extra_ticket_no == eetd_.format_.ticketformat_data_[j].extra_ticket_no_) {
							ExtraTicketAccount eta;
							eta.rep_->td_ = eetd_.master_.ticket_data_[i];
							eta.rep_->tfd_ = eetd_.format_.ticketformat_data_[j];
							if(eta.sale_judge_cm(date, days, pre)) { ++account_n;}
							break;
						}
					}
					break;
				}
			}
		}
		return account_n;
	}


	//口座情報取得（次候補）
	bool next_one(ExtraTicketAccount& eta, bool collaboration) {
		bool ret = false;
		if(collaboration) {
		  for(;next_cnt_sp_ >= 0 && next_cnt_sp_ < ACCOUNT_NUM;){
			unsigned short extra_ticket_no = ecd_.button_display_order_sp_[next_cnt_sp_];
			// 企画券マスタ中に検索
			for(int i = 0; i < ecd_.master_record_count_; ++i) {
			  if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_) {
				// 企画券フォーマットデータ中に検索
				for(int j = 0; j < eetd_.format_.ticketformat_data_.size(); ++j) {
				  if(extra_ticket_no == eetd_.format_.ticketformat_data_[j].extra_ticket_no_) {
					eta.rep_->td_ = eetd_.master_.ticket_data_[i];
					eta.rep_->tfd_ = eetd_.format_.ticketformat_data_[j];
					ret = true;
					break;
				  }
				}
				eta.rep_->td_.button_index_ = (unsigned short)next_cnt_sp_;
				break;
			  }
			}
			++next_cnt_sp_;
			if(ret) break;
		  }
		} else {
		  for(;next_cnt_ >= 0 && next_cnt_ < ACCOUNT_NUM;){
			unsigned short extra_ticket_no = ecd_.button_display_order_[next_cnt_];
			// 企画券マスタ中に検索
			for(int i = 0; i < ecd_.master_record_count_; ++i) {
			  if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_) {
				// 企画券フォーマットデータ中に検索
				for(int j = 0; j < eetd_.format_.ticketformat_data_.size(); ++j) {
				  if(extra_ticket_no == eetd_.format_.ticketformat_data_[j].extra_ticket_no_) {
					eta.rep_->td_ = eetd_.master_.ticket_data_[i];
					eta.rep_->tfd_ = eetd_.format_.ticketformat_data_[j];
					ret = true;
					break;
				  }
				}
				eta.rep_->td_.button_index_ = (unsigned short)next_cnt_;
				break;
			  }
			}
			++next_cnt_;
			if(ret) break;
		  }
		}
		return ret;
	}

  void first(bool collaboration){ 
    if(collaboration) {
      next_cnt_sp_ = 0; 
    } else {
      next_cnt_ = 0; 
    }
  }
  //口座情報取得（企画券No指定）
  bool get_account_ticket_no(unsigned short extra_ticket_no, ExtraTicketAccount& eta, bool collaboration) const{
    bool ret = false;
    // 口座ボタン表示順序中に検索
    for(int k = 0; k < ACCOUNT_NUM; ++k){
      unsigned short button_extra_ticket_no = 0;
      if(collaboration) {
        button_extra_ticket_no = ecd_.button_display_order_sp_[k];
      } else {
        button_extra_ticket_no = ecd_.button_display_order_[k];
      }
      if(extra_ticket_no == button_extra_ticket_no){
        // 企画券マスタ中に検索
        for(int i = 0; i < ecd_.master_record_count_; ++i){
          if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_){
            // 企画券フォーマットデータ中に検索
            for(int j = 0; j < eetd_.format_.ticketformat_data_.size(); ++j){
              if(extra_ticket_no == eetd_.format_.ticketformat_data_[j].extra_ticket_no_){
                eta.rep_->td_ = eetd_.master_.ticket_data_[i];
                eta.rep_->tfd_ = eetd_.format_.ticketformat_data_[j];
                ret = true;
                break;
              }
            }
            break;
          }
        }
        break;
      }
    }
    return ret;
  }
  //口座情報取得（企画券No指定　窓処払戻用）
  bool get_account_ticket_no_refund_cm(unsigned short extra_ticket_no, ExtraTicketAccount& eta) const{
    bool ret = false;
    // 企画券マスタ中に検索
    for(int i = 0; i < ecd_.master_record_count_; ++i){
      if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_){
        // 企画券フォーマットデータ中に検索
        for(int j = 0; j < eetd_.format_.ticketformat_data_.size(); ++j){
          if(extra_ticket_no == eetd_.format_.ticketformat_data_[j].extra_ticket_no_){
            eta.rep_->td_ = eetd_.master_.ticket_data_[i];
            eta.rep_->tfd_ = eetd_.format_.ticketformat_data_[j];
            ret = true;
            break;
          }
        }
        break;
      }
    }
    return ret;
  }

  ////企画券表示設定（右）(券売機用)
  //bool get_right_button(ExtraTicketAccount& eta, bool collaboration) const{
  //  bool ret = false;
  //  unsigned short extra_ticket_no = 0;
  //  if(collaboration) {
  //    extra_ticket_no = ecd_.right_display_sp_;
  //  } else {
  //    extra_ticket_no = ecd_.right_display_;
  //  }
  //  // 口座ボタン表示順序中に検索
  //  for(int k = 0; k < ACCOUNT_NUM; ++k){
  //    unsigned short button_extra_ticket_no = 0;
  //    if(collaboration) {
  //      button_extra_ticket_no = ecd_.button_display_order_sp_[k];
  //    } else {
  //      button_extra_ticket_no = ecd_.button_display_order_[k];
  //    }
  //    if(extra_ticket_no == button_extra_ticket_no){
  //      // 企画券マスタ中に検索
  //      for(int i = 0; i < ecd_.master_record_count_; ++i){
  //        if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_){
  //          // 企画券フォーマットデータ中に検索
  //          for(int j = 0; j < eetd_.format_.ticketformat_data_.size(); ++j){
  //            if(extra_ticket_no == eetd_.format_.ticketformat_data_[j].extra_ticket_no_){
  //              eta.rep_->td_ = eetd_.master_.ticket_data_[i];
  //              eta.rep_->tfd_ = eetd_.format_.ticketformat_data_[j];
  //              ret = true;
  //              break;
  //            }
  //          }
  //          break;
  //        }
  //      }
  //      break;
  //    }
  //  }
  //  return ret;
  //}
  ////企画券表示設定（左）(券売機用)
  //bool get_left_button(ExtraTicketAccount& eta, bool collaboration) const{
  //  bool ret = false;
  //  unsigned short extra_ticket_no = 0;
  //  if(collaboration) {
  //    extra_ticket_no = ecd_.left_display_sp_;
  //  } else {
  //    extra_ticket_no = ecd_.left_display_;
  //  }
  //  // 口座ボタン表示順序中に検索
  //  for(int k = 0; k < ACCOUNT_NUM; ++k){
  //    unsigned short button_extra_ticket_no = 0;
  //    if(collaboration) {
  //      button_extra_ticket_no = ecd_.button_display_order_sp_[k];
  //    } else {
  //      button_extra_ticket_no = ecd_.button_display_order_[k];
  //    }
  //    if(extra_ticket_no == button_extra_ticket_no){
  //      // 企画券マスタ中に検索
  //      for(int i = 0; i < ecd_.master_record_count_; ++i){
  //        if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_){
  //          // 企画券フォーマットデータ中に検索
  //          for(int j = 0; j < eetd_.format_.ticketformat_data_.size(); ++j){
  //            if(extra_ticket_no == eetd_.format_.ticketformat_data_[j].extra_ticket_no_){
  //              eta.rep_->td_ = eetd_.master_.ticket_data_[i];
  //              eta.rep_->tfd_ = eetd_.format_.ticketformat_data_[j];
  //              ret = true;
  //              break;
  //            }
  //          }
  //          break;
  //        }
  //      }
  //      break;
  //    }
  //  }
  //  return ret;
  //}

	EachExtraTicketMasterRep() : next_cnt_(0), next_cnt_sp_(0), cnt_(1) {}
	virtual ~EachExtraTicketMasterRep(){ cnt_ = -1; }

	EachExtraTicketMasterRep* dup() {
		EachExtraTicketMasterRep* ret = 0;
		if (cnt_ > 0) {
			++cnt_;
			ret = this;
		}
		return ret;
	}

	void rel() {
		if (--cnt_ == 0) {
			delete this;
		}
	}

	EachExtraTicketData eetd_;
	EachComData ecd_;
	int next_cnt_;
	int next_cnt_sp_;
	int cnt_;
};

EachExtraTicketMaster::EachExtraTicketMaster() : rep_(new EachExtraTicketMasterRep()) {}

EachExtraTicketMaster::EachExtraTicketMaster(const EachExtraTicketMaster& c) : rep_(c.rep_) {
	if (rep_) {
		rep_->dup();
	}
}

EachExtraTicketMaster& EachExtraTicketMaster::operator=(const EachExtraTicketMaster& c) {
	if (this != &c) {
		if (rep_) {
			rep_->rel();
			rep_ = 0;
		}
		if (c.rep_) {
			rep_ = c.rep_->dup();
		}
	}
	return *this;
}

EachExtraTicketMaster::~EachExtraTicketMaster() {
	if (rep_) {
		rep_->rel();
		rep_ = 0;
	}
}

//unsigned short EachExtraTicketMaster::account_number_v(Timestamp date, int days, bool collaboration, bool pre) const{
//  if(rep_){ return rep_->account_number_v(date, days, collaboration, pre); }
//  else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
//}

unsigned short EachExtraTicketMaster::account_number_cm(Timestamp date, int days, bool collaboration, bool pre) const{
  if(rep_){ return rep_->account_number_cm(date, days, collaboration, pre); }
  else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool EachExtraTicketMaster::next_one(ExtraTicketAccount& eta, bool collaboration){
  if(rep_){ return rep_->next_one(eta, collaboration); }
  else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

void EachExtraTicketMaster::first(bool collaboration){
  if(rep_){ rep_->first(collaboration); }
  else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool EachExtraTicketMaster::get_account_ticket_no(unsigned short extra_ticket_no, ExtraTicketAccount& eta, bool collaboration) const{
  if(rep_){ return rep_->get_account_ticket_no(extra_ticket_no, eta, collaboration); }
  else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

bool EachExtraTicketMaster::get_account_ticket_no_refund_cm(unsigned short extra_ticket_no, ExtraTicketAccount& eta) const{
  if(rep_){ return rep_->get_account_ticket_no_refund_cm(extra_ticket_no, eta); }
  else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}

//bool EachExtraTicketMaster::get_right_button(ExtraTicketAccount& eta, bool collaboration) const{
//  if(rep_){ return rep_->get_right_button(eta, collaboration); }
//  else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
//}
//
//bool EachExtraTicketMaster::get_left_button(ExtraTicketAccount& eta, bool collaboration) const{
//  if(rep_){ return rep_->get_left_button(eta, collaboration); }
//  else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
//}

class ExtraTicketMasterRep{
	friend class ExtraTicketMaster;

	//マスタ名取得
	const char* name() const { return MasterDef::EXTRA_TICKET; }

	//マスタファイル名取得
	const char* getMasterFileName() const {
		return FILE_PATH;
	}

	//設定バージョン取得
	const char* version() const {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::version() > ");
		const char* ret = MasterDef::UNKNOWN;
//		if(data_.size()){ ret = etd_.prefix_.st_set_version_.c_str(); }
		OMRONSOFT_TRACE("ExtraTicketMasterRep::version() < ");
		return ret;
	}

	////マスタバージョン取得
	//const char* master_version() const {
	//	OMRONSOFT_TRACE("ExtraTicketMasterRep::master_version() > ");
	//	const char* ret = MasterDef::UNKNOWN;
	//	if(data_.size()){ ret = etd_.prefix_.st_master_version_.c_str(); }
	//	OMRONSOFT_TRACE("ExtraTicketMasterRep::master_version() < ");
	//	return ret;
	//}

	//企画券データバージョン取得
	const char* master_version() const {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::master_version() > ");
		const char* ret = MasterDef::UNKNOWN;
		if(data_.size()){
			// システム時間により、有効な企画券マスタ取得
			OpDateTime opDate(Timestamp::current());

			Timestamp com_date_op = opDate.auto_farechange();
			OMRONSOFT_TRACE("判定日付時刻:" << com_date_op.year() << "年" <<
			com_date_op.month() << "月" << com_date_op.day() << "日");
			
			int ver(0);
			if(master_no(com_date_op,ver)) {
				ret = etd_.comprefix_.comdata_[ver].st_data_version_.c_str();
			}
		}
		OMRONSOFT_TRACE("ExtraTicketMasterRep::master_version() < ");
		return ret;
	}

	//企画券データ(配信データ)作成日取得
	int master_data_delivery_time(Timestamp& delivery_time) const {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::master_data_delivery_time() > ");
		int ret = ExtraTicketMaster::NG;
		if(data_.size()){ 
			delivery_time = etd_.comprefix_.data_delivery_time_;
			ret = ExtraTicketMaster::OK;
		}else{
			//内部データ異常
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}
		OMRONSOFT_TRACE("ExtraTicketMasterRep::master_data_delivery_time() < ");
		return ret;
	}


	////サム値取得
	//unsigned long sum() const {
	//	OMRONSOFT_TRACE("ExtraTicketMasterRep::sum() > ");
	//	unsigned long ret = 0;

	//	if(data_.size()) {
	//		ret = etd_.prefix_.sum_;
	//	}

	//	OMRONSOFT_TRACE("ExtraTicketMasterRep::sum() < ");
	//	return ret;
	//}

	//サム値計算
	unsigned long sum() const {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::sum() > ");
		unsigned long ret = 0;

		if(data_.size()) {
			const char* p = data_.c_str();
			for (unsigned long l=0; l < data_.size(); l++) {
				ret += unsigned long(*p);
			}
		}

		OMRONSOFT_TRACE("ExtraTicketMasterRep::sum() < ");
		return ret;
	}

	//マスタデータセット
	int data_set(const char* data, long size){
		OMRONSOFT_TRACE("ExtraTicketMasterRep::data_set() > ");
		int ret = ExtraTicketMaster::NG;
		// data_sizeは使わない。
		long data_size = 0;
		int error_detail = ERROR_OTHER;
		string buff_data;
		data_ = "";
		if(etd_.internalize(data, data_size, error_detail)){
			string dat1_path = ConstVal::instance().dat1_path();// DAT1パス取得
			string dat2_path = ConstVal::instance().dat2_path();// DAT2パス取得
			DoubleData dd(true, dat1_path.c_str(), dat2_path.c_str());// 二重データ管理クラス
			string file_path = dat1_path + string(FILE_PATH);   // 絶対パス作成
//			buff_data = string(data,data_size); // 通信部ヘッダがないので、データサイズ取得できません
			buff_data = string(data,size);

			// 二重化用データセット
			DD_RES eRes = dd.DoubleSetData(file_path.c_str(), buff_data.data(),
			                               buff_data.size(), ios::binary );

			if(eRes == DD_RES_OK) {
				// データセット成功
				data_ = buff_data;
				ret = ExtraTicketMaster::OK;
			} else {
				// データセット失敗
				// 異常詳細を取得
				int iDat1Err = dd.GetErrDetail(true);
				int iDat2Err = dd.GetErrDetail(false);
				// 異常を通知する
				if(iDat1Err) {
					NOTICE_DBLDATA_ERROR(iDat1Err," DataSet Error ",file_path);
				}
				if(iDat2Err) {
					NOTICE_DBLDATA_ERROR(iDat2Err," DataSet Error ",dd.CvtDat1ToDat2Path(file_path.c_str()));
				}
				THROW_ERROR(MasterDef::WRITE_ERR,"Data Format Error",__FILE__,__LINE__)
			}
		}else{
			//データフォーマットエラー
			OMRONSOFT_TRACE("Data Format Error");
			THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"Data Format Error",__FILE__,__LINE__)
		}
		OMRONSOFT_TRACE("ExtraTicketMaster::data_set() < ");
		return ret;
	}

	//マスタデータ取得
	int data_get(const char*& data, long& size) const{
		OMRONSOFT_TRACE("ExtraTicketMasterRep::data_get() > ");
		int ret = ExtraTicketMaster::NG;
		if(data_.size()){
			data = data_.data();
			size = data_.size();
			ret = ExtraTicketMaster::OK;
		}else{
			//内部データ異常
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}

		OMRONSOFT_TRACE("ExtraTicketMasterRep::data_get() < ");
		return ret;
	}

	//有効開始年月日時分取得（マスタ中のmaster_id番目のデータの有効開始日を取得）
	int valid_date(int master_id, Timestamp& valid_date) const {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::valid_date() > ");
		int ret = ExtraTicketMaster::NG;

		if(data_.size()){
			if(master_id > 0 && master_id <= MASTER_NUM){
				valid_date = etd_.comprefix_.comdata_[master_id - 1].valid_date_;
				ret = ExtraTicketMaster::OK;
			}
		}else{
			//内部データ異常
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}

		OMRONSOFT_TRACE("ExtraTicketMasterRep::valid_date() < ");
		return ret;
	}

	//有効開始年月日時分およびバージョン取得（マスタ中のmaster_id番目のデータの有効開始日およびバージョンを取得）
	int valid_master(int master_id, Timestamp& valid_date, const char*& master_version) const {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::valid_date() > ");
		int ret = ExtraTicketMaster::NG;

		if(data_.size()){
			if(master_id > 0 && master_id <= MASTER_NUM){
				valid_date = etd_.comprefix_.comdata_[master_id - 1].valid_date_;
				master_version = etd_.comprefix_.comdata_[master_id - 1].st_data_version_.c_str();
				ret = ExtraTicketMaster::OK;
			}
		}else{
			//内部データ異常
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}

		OMRONSOFT_TRACE("ExtraTicketMasterRep::valid_date() < ");
		return ret;
	}

	//有効マスタ取得
	int get_master(const Timestamp& real_date, EachExtraTicketMaster& valid_master) {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::get_master() > ");
		int ret = ExtraTicketMaster::NG;
		int new_id = 0;
		int old_id = 0;
		Timestamp real_date_tmp(real_date);
		OpDateTime com_date(real_date_tmp);
		Timestamp com_date_op = com_date.auto_farechange();
		OMRONSOFT_TRACE("判定日付時刻:" << com_date_op.year() << "年" <<
			com_date_op.month() << "月" << com_date_op.day() << "日");
		if(data_.size()){
			//マスタ１，２のうちどちらが新しいかを調べる
			//同じ時は１を新しいマスタとする
			if(etd_.comprefix_.comdata_[0].valid_date_ >= etd_.comprefix_.comdata_[1].valid_date_){
				new_id = 0; old_id = 1;
			}else{
				new_id = 1; old_id = 0;
			}
			OMRONSOFT_TRACE("新マスタ：マスタ" << new_id + 1 <<
				"、旧マスタ：マスタ" << old_id + 1);

			OMRONSOFT_TRACE("マスタ１日付時刻:" <<
				etd_.comprefix_.comdata_[0].valid_date_.year() << "年" <<
				etd_.comprefix_.comdata_[0].valid_date_.month() << "月" <<
				etd_.comprefix_.comdata_[0].valid_date_.day() << "日" <<
				etd_.comprefix_.comdata_[0].valid_date_.hour() << "時" <<
				etd_.comprefix_.comdata_[0].valid_date_.minute() << "分" <<
				etd_.comprefix_.comdata_[0].valid_date_.second() << "秒");
			OMRONSOFT_TRACE("マスタ２日付時刻:" <<
				etd_.comprefix_.comdata_[1].valid_date_.year() << "年" <<
				etd_.comprefix_.comdata_[1].valid_date_.month() << "月" <<
				etd_.comprefix_.comdata_[1].valid_date_.day() << "日" <<
				etd_.comprefix_.comdata_[1].valid_date_.hour() << "時" <<
				etd_.comprefix_.comdata_[1].valid_date_.minute() << "分" <<
				etd_.comprefix_.comdata_[1].valid_date_.second() << "秒");
			//マスタ１，２のうちどちらが有効かを調べる
			//まず、新しい方から
			//新しい方でなければ古いほうを調べる
			if(com_date_op >= etd_.comprefix_.comdata_[new_id].valid_date_){
				OMRONSOFT_TRACE("有効マスタあり：マスタ" << new_id + 1);
				valid_master.rep_->eetd_ = etd_.eachdata_[new_id];
				valid_master.rep_->ecd_ = etd_.comprefix_.comdata_[new_id];
				update_kind_map(true);
				ret = ExtraTicketMaster::OK;
			}else if(com_date_op >= etd_.comprefix_.comdata_[old_id].valid_date_){
				OMRONSOFT_TRACE("有効マスタあり：マスタ" << old_id + 1);
				valid_master.rep_->eetd_ = etd_.eachdata_[old_id];
				valid_master.rep_->ecd_ = etd_.comprefix_.comdata_[old_id];
				update_kind_map(true);
				ret = ExtraTicketMaster::OK;
			}

		}else{
			//内部データ異常
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}

		OMRONSOFT_TRACE("ExtraTicketMasterRep::get_master() < ");
		return ret;
	}

	bool update_kind_map(bool needjudge) {
		bool ret = false;
		int new_id = 0;
		int old_id = 0;
		int cur_id = 0;
		SysTime::time tm = SysTime::time();
		SysTime::get_localtime(tm);
		Timestamp nowtime(tm.year, tm.month, tm.day, tm.hour, tm.minute, tm.second);
		OpDateTime com_date(nowtime);
		Timestamp com_date_op = com_date.auto_farechange();
		ExtraTicketMaster& master = dynamic_cast<ExtraTicketMaster&>(MasterDataMgr::instance().get_master(MasterDef::EXTRA_TICKET));
		EachExtraTicketMaster valid_master;
		ExtraTicketAccount    eta;
		EachExtraTicketMaster old_master;
		if(data_.size()){
			//マスタ１，２のうちどちらが新しいかを調べる
			//同じ時は１を新しいマスタとする
			if(etd_.comprefix_.comdata_[0].valid_date_ >= etd_.comprefix_.comdata_[1].valid_date_){
				new_id = 0; old_id = 1;
			}else{
				new_id = 1; old_id = 0;
			}
			OMRONSOFT_TRACE("新マスタ：マスタ" << new_id + 1 <<
				"、旧マスタ：マスタ" << old_id + 1);

			//マスタ１，２のうちどちらが有効かを調べる
			//まず、新しい方から
			//新しい方でなければ古いほうを調べる
			if(com_date_op >= etd_.comprefix_.comdata_[new_id].valid_date_){
				OMRONSOFT_TRACE("有効マスタあり：マスタ" << new_id + 1);
				valid_master.rep_->eetd_ = etd_.eachdata_[new_id];
				valid_master.rep_->ecd_ = etd_.comprefix_.comdata_[new_id];
				cur_id = new_id + 1;
			}else if(com_date_op >= etd_.comprefix_.comdata_[old_id].valid_date_){
				OMRONSOFT_TRACE("有効マスタあり：マスタ" << old_id + 1);
				valid_master.rep_->eetd_ = etd_.eachdata_[old_id];
				valid_master.rep_->ecd_ = etd_.comprefix_.comdata_[old_id];
				cur_id = old_id + 1;
			} else {
				return ret;
			}

			if(master.m_CurMasterID != cur_id || !needjudge){
				unsigned short i = 1;
				ExtraTicketMaster::KindElement onekind;
				ExtraTicketMaster::PlanDataKind temp;
				master.m_DataKindMap.clear();
				valid_master.first(false);
				while(valid_master.next_one(eta,false)){
	  				temp.dataKindNo = eta.issue_data_kind_no();
		  			temp.dataNo     = eta.issue_data_no();
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.issue_data_kind_no();
		  			temp.dataNo     = eta.issue_data_no() + 1;
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.refund_data_kind_no();
					temp.dataNo     = eta.refund_data_no();
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.refund_data_kind_no();
					temp.dataNo     = eta.refund_data_no() + 1;
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
				}
				valid_master.first(true);
				while(valid_master.next_one(eta,true)){
	  				temp.dataKindNo = eta.issue_data_kind_no();
		  			temp.dataNo     = eta.issue_data_no();
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.issue_data_kind_no();
		  			temp.dataNo     = eta.issue_data_no() + 1;
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.refund_data_kind_no();
					temp.dataNo     = eta.refund_data_no();
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.refund_data_kind_no();
					temp.dataNo     = eta.refund_data_no() + 1;
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
				}

				master.m_CurMasterID = cur_id;

				if(master.m_CurMasterID == new_id + 1){
					old_master.rep_->eetd_ = etd_.eachdata_[old_id];
				    old_master.rep_->ecd_  = etd_.comprefix_.comdata_[old_id];
				}
				else if(master.m_CurMasterID == old_id + 1){
					old_master.rep_->eetd_ = etd_.eachdata_[new_id];
				    old_master.rep_->ecd_  = etd_.comprefix_.comdata_[new_id];
				}
				else{
					return ret;
				}

				old_master.first(false);
				while(old_master.next_one(eta,false)){
  					temp.dataKindNo = eta.issue_data_kind_no();
	  				temp.dataNo     = eta.issue_data_no();
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.issue_data_kind_no();
	  				temp.dataNo     = eta.issue_data_no() + 1;
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.refund_data_kind_no();
					temp.dataNo     = eta.refund_data_no();
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.refund_data_kind_no();
					temp.dataNo     = eta.refund_data_no() + 1;
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
				}
				old_master.first(true);
				while(old_master.next_one(eta,true)){
  					temp.dataKindNo = eta.issue_data_kind_no();
	  				temp.dataNo     = eta.issue_data_no();
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.issue_data_kind_no();
	  				temp.dataNo     = eta.issue_data_no() + 1;
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.refund_data_kind_no();
					temp.dataNo     = eta.refund_data_no();
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
					temp.dataKindNo = eta.refund_data_kind_no();
					temp.dataNo     = eta.refund_data_no() + 1;
					onekind.first = i;
					onekind.second = temp;
					master.m_DataKindMap.insert(onekind);
					i++;
				}
			}
			ret = true;
		}else{
			//内部データ異常
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}
		return ret;
	}


	//設置駅コード取得
	unsigned long station_setting_code() const{
		OMRONSOFT_TRACE("ExtraTicketMasterRep::station_setting_code() > ");
		unsigned long ret = ExtraTicketMaster::NG;
		if(data_.size()){ ret = etd_.comprefix_.station_setting_code_; }
		OMRONSOFT_TRACE("ExtraTicketMasterRep::station_setting_code() < ");
		return ret;
	}



	//デバッグ用
	//    void print() {
	//      ExtraTicketMasterData etmd;
	//      if(etmd.internalize(data_.data(),data_.size())){
	//        cout << "[ヘッダ]" << std::endl;
	//        cout << " 設定ﾊﾞｰｼﾞｮﾝ ﾏｽﾀﾊﾞｰｼﾞｮﾝ  ﾃﾞｰﾀｻｲｽﾞ    サム値" << std::endl;
	//        cout << setw(11) << etmd.prefix_.st_version_;
	//        cout << setw(10) << (unsigned int)etmd.prefix_.master_version_;
	//        cout << setw(10) << etmd.prefix_.size_;
	//        cout << setw(11) << etmd.prefix_.sum_ << std::endl;;

	//        for(int i = 0; i < MASTER_NUM; ++i){
	//          EachMasterData e;
	//          e = etmd.master_[i];
	//          cout << "[マスタ" << i+1 << "]  有効開始年月日時分:"
	//               << e.valid_date_.year() << "/" << e.valid_date_.month() << "/"
	//               << e.valid_date_.day()  << " " << e.valid_date_.hour()  << ":"
	//               << e.valid_date_.minute() << std::endl;
	//          cout << "口座" << "                             " << "                 " << "                 " << "ﾎﾞﾀﾝ"
	//               << "  発売    "<< "  発売    "<< " 事前 "<< " 有効 "<< "   有効   "<< " 発売 "<< " 合計 "<< " 合計 "
	//               << "印字 " << " 大人" << " 小児" << " 磁気" << "  発駅  " << "  発駅  " << "  発駅  " << "券種"
	//               << " 共通" << " 打" << " 運" << " 連" << " 下" << " 制" << " 併" << " 往" << "  ﾌﾘｰ" << " 途"
	//               << " 優" << " 着駅" << " 着駅" << " 発駅" << " 発駅" << "  経由  " << "  経由  " << "  経由  "
	//               << "  経由  " << "  経由  " << "  経由  " << "  経由  " << "  経由  " << "  経由  " << "  経由  "
	//               << "  着駅  " << "  着駅  " << "  着駅  " << "ﾌﾘｰ " <<std::endl;
	//          cout << "番号" << "             券名称          " << "  券名称（上段） " << "  券名称（下段） " << "位置"
	//               << "  開始日  "<< "  終了日  "<< " 発売 "<< " 日数 "<< "   終了日 "<< " 枚数 "<< " 大人 "<< " 小児 "
	//               << "ﾊﾟﾀｰﾝ" << " 金額" << " 金額" << "   化" << "  (１)  " << "  (２)  " << "  (３)  " << "  ３"
	//               << " 予備" << " 切" << " 改" << " 絡" << " 車" << " 限" << " 割" << " 復" << " ﾋﾞｯﾄ" << " 中"
	//               << " 等" << " 1-2 " << " ｴﾘｱ " << " 1-2 " << " ｴﾘｱ " << "  (１)  " << "  (２)  " << "  (３)  "
	//               << "  (４)  " << "  (５)  " << "  (６)  " << "  (７)  " << "  (８)  " << "  (９)  " << "  (10)  "
	//               << "  (１)  " << "  (２)  " << "  (３)  " << "区間" <<std::endl;

	//          for(int j = 0; j < ACCOUNT_NUM; ++j){
	//            AccountData ac;
	//            ac = e.account_[j];
	//            cout << setw(4)  << ac.account_no_ << setw(27) << ac.ticket_name_
	//                 << setw(17) << ac.ticket_name_upper_ << setw(17) << ac.ticket_name_lower_
	//                 << setw(6)  << ac.button_position_ << setw(5) << ac.sale_start_date_.year()
	//                 << setw(2) << ac.sale_start_date_.month() << setw(2) << ac.sale_start_date_.day()
	//                 << setw(6) << ac.sale_end_date_.year() << setw(2) << ac.sale_end_date_.month()
	//                 << setw(2) << ac.sale_end_date_.day() << setw(5) << ac.advance_days_
	//                 << setw(6) << ac.valid_days_ << setw(8) << ac.valid_end_date_.year()
	//                 << setw(2) << ac.valid_end_date_.month() << setw(2) << ac.valid_end_date_.day()
	//                 << setw(4) << ac.sale_number_ << setw(7) << ac.adult_sale_amount_
	//                 << setw(6) << ac.child_sale_amount_;
	//            for(int k = 0; k < ISSUE_NUM; ++k){
	//              IssueInfo ii;
	//              ii = ac.issue_info_[k];
	//              if(k>0){ cout << setw(130) << "";}
	//              cout << setw(5) << ii.print_pattern_no_ << setw(6) << ii.adult_fare_;
	//              cout << setw(5) << ii.child_fare_ << setw(5) << ii.magnetic_;
	//              cout << setw(4) << ii.start1_.line << "-" << setw(3) << ii.start1_.station;
	//              cout << setw(4) << ii.start2_.line << "-" << setw(3) << ii.start2_.station;
	//              cout << setw(4) << ii.start3_.line << "-" << setw(3) << ii.start3_.station;
	//              cout << setw(4) << ii.ticket_kind3_ << setw(3) << ii.common_reserve1_
	//                   << setw(2) << ii.common_reserve2_ << setw(3) << ii.close_fare_
	//                   << setw(3) << ii.change_fare_bit_ << setw(3) << ii.connection_
	//                   << setw(3) << ii.no_stop_over1_ << setw(3) << ii.restrict_term_
	//                   << setw(3) << ii.merger_discount_ << setw(3) << ii.round_trip_
	//                   << setw(5) << ii.free_area_bit_ << setw(3) << ii.no_stop_over2_
	//                   << setw(3) << ii.no_honor_ << setw(5) << ii.end_station_combination_
	//                   << setw(5) << ii.end_station_area_ << setw(5) << ii.start_station_combination_
	//                   << setw(5) << ii.start_station_area_ ;
	//              cout << setw(4) << ii.via1_.line << "-" << setw(3) << ii.via1_.station;
	//              cout << setw(4) << ii.via2_.line << "-" << setw(3) << ii.via2_.station;
	//              cout << setw(4) << ii.via3_.line << "-" << setw(3) << ii.via3_.station;
	//              cout << setw(4) << ii.via4_.line << "-" << setw(3) << ii.via4_.station;
	//              cout << setw(4) << ii.via5_.line << "-" << setw(3) << ii.via5_.station;
	//              cout << setw(4) << ii.via6_.line << "-" << setw(3) << ii.via6_.station;
	//              cout << setw(4) << ii.via7_.line << "-" << setw(3) << ii.via7_.station;
	//              cout << setw(4) << ii.via8_.line << "-" << setw(3) << ii.via8_.station;
	//              cout << setw(4) << ii.via9_.line << "-" << setw(3) << ii.via9_.station;
	//              cout << setw(4) << ii.via10_.line << "-" << setw(3) << ii.via10_.station;

	//              cout << setw(4) << ii.end1_.line << "-" << setw(3) << ii.end1_.station;
	//              cout << setw(4) << ii.end2_.line << "-" << setw(3) << ii.end2_.station;
	//              cout << setw(4) << ii.end3_.line << "-" << setw(3) << ii.end3_.station;
	//              cout << setw(3) << ii.free_area_code_; 
	//              cout << std::endl;
	//              cout << setw(130) << "";
	//              cout << setw(37) << ii.area_info1_ << setw(37) << ii.area_info2_
	//                   << setw(37) << ii.area_info3_ << setw(37) << ii.area_info4_
	//                   << setw(37) << ii.area_info5_ << std::endl << setw(130) << "" << setw(37) << ii.area_info6_
	//                   << setw(37) << ii.area_info7_ << setw(37) << ii.area_info8_
	//                   << setw(37) << ii.area_info9_ << setw(37) << ii.area_info10_ << std::endl << setw(130) << ""
	//                   << setw(37) << ii.area_info11_ << setw(37) << ii.area_info12_
	//                   << setw(37) << ii.area_info13_ << setw(37) << ii.area_info14_
	//                   << setw(37) << ii.area_info15_ << std::endl << setw(130) << ""<< setw(37) << ii.area_info16_
	//                   << setw(37) << ii.area_info17_ << setw(37) << ii.area_info18_ << std::endl;
	//            }

	//            cout << std::endl;
	//          }
	//          cout << std::endl;
	//        }
	//      }
	//    }

	//************************************************************************************
	// NAME       : CheckData
	// FUNCTION   : ExtraTicketMasterデータチェック処理
	// PARAM      : なし
	// RETURN     : なし
	//************************************************************************************
	void CheckData() {
		OMRONSOFT_TRACE("ExtraTicketMaster::CheckData() start ");
		string dat1_path = ConstVal::instance().dat1_path();    // DAT1パス取得
		string dat2_path = ConstVal::instance().dat2_path();    // DAT2パス取得
		DoubleData dd(true, dat1_path.c_str(), dat2_path.c_str());  // 二重データ管理クラス
		string file_path = dat1_path + string(FILE_PATH);   // 絶対パス作成
		// 二重管理用ディレクトリチェック＆作成
		DD_RES eRes = dd.DoubleMakeDirectory(file_path.c_str(), true);
		if( eRes != DD_RES_OK ) {
			// 異常詳細を取得
			int iDat1Err = dd.GetErrDetail(true);
			int iDat2Err = dd.GetErrDetail(false);
			// 異常があれば通知する
			if(iDat1Err) {
				NOTICE_DBLDATA_ERROR(iDat1Err," MakeDir Error ",file_path);
			}
			if(iDat2Err) {
				NOTICE_DBLDATA_ERROR(iDat2Err," MakeDir Error ",dd.CvtDat1ToDat2Path(file_path.c_str()));
			}
		}

		// 二重化判定・復旧
		eRes = dd.MatchingData(file_path.c_str());
		// 異常詳細を取得
		int iDat1Err = dd.GetErrDetail(true);
		int iDat2Err = dd.GetErrDetail(false);
		switch(eRes) {
			case DD_RES_OK :              // 正常終了
				// 何もしない
				break;
			case DD_RES_RECOVERY_MAIN :   // 主データ復旧
			case DD_RES_RECOVERY_SUB  :   // 従データ復旧
				// 異常を登録する（登録のみ表示はしない）
				if(iDat1Err) {
					COUNT_DBLDATA_ERROR(iDat1Err,"DataRecover : ", file_path);
				}
				if(iDat2Err) {
					COUNT_DBLDATA_ERROR(iDat2Err,"DataRecover : ", dd.CvtDat1ToDat2Path(file_path.c_str()));
				}
				break;
			case DD_RES_MAIN_NG :         // 主データファイル異常
			case DD_RES_SUB_NG  :         // 従データファイル異常
			case DD_RES_MAIN_SUB_NG :     // 主＆従データファイル異常
			case DD_RES_SUM_NG  :         // サム値不一致異常
			case DD_RES_NG :              // その他異常(引数がNULL等)
			default :
				// 異常を通知する
				if(iDat1Err) {
					NOTICE_DBLDATA_ERROR(iDat1Err,"MatchingData Error : ", file_path);
				}
				if(iDat2Err) {
					NOTICE_DBLDATA_ERROR(iDat2Err,"MatchingData Error : ", dd.CvtDat1ToDat2Path(file_path.c_str()));
				}
				break;
		}
		OMRONSOFT_TRACE("ExtraTicketMaster::CheckData() end ");
	}

	// 有効マスタ番号取得（０：新　１：現）
	bool master_no(const Timestamp& date, int& no) const {
		bool ret(false);
		int new_id = 0;
		int old_id = 0;
		//マスタ１，２のうちどちらが新しいかを調べる
		//同じ時は１を新しいマスタとする
		if(etd_.comprefix_.comdata_[0].valid_date_ >= etd_.comprefix_.comdata_[1].valid_date_){
			new_id = 0; old_id = 1;
		}else{
			new_id = 1; old_id = 0;
		}
		//マスタ１，２のうちどちらが有効かを調べる
		//まず、新しい方から
		//新しい方でなければ古いほうを調べる
		if(date >= etd_.comprefix_.comdata_[new_id].valid_date_){
			no = new_id;
			ret = true;
		}else if(date >= etd_.comprefix_.comdata_[old_id].valid_date_){
			no = old_id;
			ret = true;
		}
		return ret;
	}

	ExtraTicketMasterRep(){
		OMRONSOFT_TRACE("ExtraTicketMasterRep::ExtraTicketMasterRep() > ");
		string buff_data;
		string msg;

		//Masterデータチェック
		CheckData();

		string dat1_path = ConstVal::instance().dat1_path();// DAT1パス取得
        string file_path = dat1_path + string(FILE_PATH);   // 絶対パス作成
		data_ = "";
		ifstream ifs(file_path.c_str(),ios::binary);
		if(ifs){
			char ch;
			while(ifs.get(ch)){
				buff_data += ch;
			}
			long size = 0;
			int error_detail = ERROR_OTHER;
			if(etd_.internalize(buff_data.data(), size, error_detail)){
				OMRONSOFT_TRACE("★★★ Format OK!! ★★★");
				data_ = buff_data;
			}else{
				msg = "File Format Error ";
				msg += FILE_PATH;
				OMRONSOFT_TRACE(msg.c_str());
				//起動時はファイルがフォーマット異常でも異常にしない
				//INTERNAL_ERROR(0, ErrOccur::MTC_EXTRA_TICKET_MASTER_ERR, msg.c_str());
			}
		}else{
			msg ="File Open Error ";
			msg += FILE_PATH;
			OMRONSOFT_TRACE(msg.c_str());
			//起動時はファイルが開けなくても異常にしない
			//INTERNAL_ERROR(0, ErrOccur::MTC_EXTRA_TICKET_MASTER_ERR, msg.c_str());
		}

		OMRONSOFT_TRACE("ExtraTicketMasterRep::ExtraTicketMasterRep() < ");
	}


	virtual ~ExtraTicketMasterRep(){}
	ExtraTicketMasterRep(const ExtraTicketMasterRep&);
	ExtraTicketMasterRep& operator=(const ExtraTicketMasterRep&);

	string data_;
	ExtraTicketData etd_;
	static const char* const FILE_PATH;
};
}
}
}

const char* const ExtraTicketMasterRep::FILE_PATH = "master\\extra_ticket.mst";

ExtraTicketMaster::ExtraTicketMaster() : rep_(new ExtraTicketMasterRep), m_CurMasterID(0){ 
	activate(); 
	update_kind_map(false);
}

ExtraTicketMaster::~ExtraTicketMaster(){
	inactivate();
	if(rep_) { delete rep_; rep_ = 0; }
}

const char* ExtraTicketMaster::name() const{
	const char* ret = MasterDef::UNKNOWN;
	if(rep_){ ret = rep_ -> name(); }
	return ret;
}

const char* ExtraTicketMaster::getMasterFileName() const{
	const char* ret = NULL;
	if(rep_){
		ret = rep_ -> getMasterFileName();
	}
	return ret;
}
const char* ExtraTicketMaster::version() const{
	const char* ret = MasterDef::UNKNOWN;
	if(rep_){ ret = rep_ -> version(); }
	return ret;
}

const char* ExtraTicketMaster::master_version() const{
	const char* ret = MasterDef::UNKNOWN;
	if(rep_){ ret = rep_ -> master_version(); }
	return ret;
}

unsigned long ExtraTicketMaster::sum() const {
	unsigned long ret = 0;
	if(rep_) {
		ret = rep_ -> sum();
	}
	return ret;
}

int ExtraTicketMaster::data_set(const char* data, long size){
	int ret = NG;
	if(rep_){ ret = rep_ -> data_set(data, size); }
	return ret;
}

int ExtraTicketMaster::data_get(const char*& data, long& size) const{
	int ret = NG;
	if(rep_){ ret = rep_ -> data_get(data, size); }
	return ret;
}

int ExtraTicketMaster::valid_date(int master_id, Timestamp& valid_date) const{
	int ret = NG;
	if(rep_){ ret = rep_ -> valid_date(master_id, valid_date); }
	return ret;
}

int ExtraTicketMaster::valid_master(int master_id, Timestamp& valid_date, const char*& master_version) const{
	int ret = NG;
	if(rep_){ ret = rep_ -> valid_master(master_id, valid_date, master_version); }
	return ret;
}

int ExtraTicketMaster::get_master(const Timestamp& real_date, EachExtraTicketMaster& valid_master) {
	int ret = NG;
	if(rep_){ ret = rep_ -> get_master(real_date, valid_master); }
	return ret;
}

unsigned long ExtraTicketMaster::station_setting_code() const{
	unsigned long ret = NG;
	if(rep_){ ret = rep_ -> station_setting_code(); }
	return ret;
}

bool ExtraTicketMaster::update_kind_map(bool needjudge) {
	bool ret = false;
	if(rep_){ ret = rep_ -> update_kind_map(needjudge); }
	return ret;
}

int ExtraTicketMaster::master_data_delivery_time(Timestamp& delivery_time) const{
	if(rep_){ return rep_ -> master_data_delivery_time(delivery_time); }
	else{ THROW_ERROR(MasterDef::LOGIC_ERR,"rep_ not exist",__FILE__,__LINE__); }
}
//void ExtraTicketMaster::print(){ if(rep_){ rep_ -> print(); } }

ExtraTicketMaster ExtraTicketMaster::theInstance;

