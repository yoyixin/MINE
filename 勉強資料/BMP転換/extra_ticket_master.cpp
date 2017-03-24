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

//�ُ�𓊂���
#define THROW_ERROR(A,B,C,D) \
	omronsoft::exception::ErrorCode ec; \
	ec.code = MasterDef::ERROR_ID; \
	ec.detail = A; \
	throw omronsoft::exception::Error(ec, B, C, D);

// ��[(H11.0) �i�S�j ���D] Start ADD
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
// ��[(H11.0) �i�S�j ���D] End   ADD


namespace omronsoft {
namespace pub {
namespace extraticketmaster {
class ExtraTicketAccountRep{
	friend class ExtraTicketAccount;
	friend class EachExtraTicketMasterRep;


	//��挔No
	unsigned short extra_ticket_no() const { return td_.extra_ticket_no_; }

	//�����f�[�^���No
	unsigned short issue_data_kind_no() const { return td_.issue_data_kind_no_; }

	//�����f�[�^No
	unsigned short issue_data_no() const { return td_.issue_data_no_; }

	//���߃f�[�^���No
	unsigned short refund_data_kind_no() const { return td_.refund_data_kind_no_; }

	//���߃f�[�^No
	unsigned short refund_data_no() const { return td_.refund_data_no_; }

	//�������z�i��l�j
	unsigned short issue_amount_adult() const { return td_.issue_amount_adult_; }

	//�������z�i�����j
	unsigned short issue_amount_child() const { return td_.issue_amount_child_; }

	//n���ڕ\���^���i��l�j
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

	//n���ڕ\���^���i�����j
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

	//���s����
	unsigned short issue_num() const { return td_.issue_num_; }

	//�����J�n�N����
	Timestamp issue_start_date() const { return td_.issue_start_date_; }

	//�����I���N����
	Timestamp issue_end_date() const { return td_.issue_end_date_; }

	//�������ł��邩�ǂ���
	bool is_indefinite() const { return td_.is_indefinite_; }

	//�O���J�n�N����
	Timestamp pre_issue_start_date() const { return td_.pre_issue_start_date_; }

	//�O�������邩�ǂ���
	bool is_pre_issue() const { return td_.is_pre_issue_; }

//	//���@�\�����@�p�L������
//	unsigned short v_valid_date_count() const { return td_.v_valid_date_count_; }

	//�����p�L�������L������
	unsigned short cm_valid_date_count() const { return td_.cm_valid_date_count_; }

	//�C�x���g�Œ�L���J�n��
	Timestamp event_start_date() const { return td_.event_start_date_; }

	//�C�x���g�Œ�L���I����
	Timestamp event_end_date() const { return td_.event_end_date_; }

//	//���@�\�����@������ ������OFF�̏ꍇ�A�����A���߂Ƃ��ɕs��
//	bool v_issue_permit() const { return td_.v_issue_permit_; }
//
//	//���@�\�����@���߉� ������OFF�̏ꍇ�A�����A���߂Ƃ��ɕs�� ���G���R�[�h�������͂P���̂�
//	bool v_refund_permit() const { return td_.v_refund_permit_ && td_.v_issue_permit_ && tfd_.encode_num_ == 1; }

	//���������@������ ���G���R�[�h�������͂P���̂�
	bool cm_issue_permit() const { return td_.cm_issue_permit_; }

	//���������@���߉�
	bool cm_refund_permit() const { return td_.cm_refund_permit_ && tfd_.encode_num_ == 1; }

	//�y�x��
	bool is_holiday() const { return td_.is_holiday_; }

	//��挔�����ݒ�i�����j�J�n��
	unsigned short limit_weekday_set_start_hour() const { return td_.limit_weekday_set_start_hour_; }

	//��挔�����ݒ�i�����j�J�n��
	unsigned short limit_weekday_set_start_minute() const { return td_.limit_weekday_set_start_minute_; }

	//��挔�����ݒ�i�����j�I����
	unsigned short limit_weekday_set_end_hour() const { return td_.limit_weekday_set_end_hour_; }

	//��挔�����ݒ�i�����j�I����
	unsigned short limit_weekday_set_end_minute() const { return td_.limit_weekday_set_end_minute_; }

	//��挔�����ݒ�i�x���j�J�n��
	unsigned short limit_holiday_set_start_hour() const { return td_.limit_holiday_set_start_hour_; }

	//��挔�����ݒ�i�x���j�J�n��
	unsigned short limit_holiday_set_start_minute() const { return td_.limit_holiday_set_start_minute_; }

	//��挔�����ݒ�i�x���j�I����
	unsigned short limit_holiday_set_end_hour() const { return td_.limit_holiday_set_end_hour_; }

	//��挔�����ݒ�i�x���j�I����
	unsigned short limit_holiday_set_end_minute() const { return td_.limit_holiday_set_end_minute_; }
//
//	//�����\�`�F�b�N�i�����@�p�j true:�����Afalse:�����s��
//	// INPUT:Timestamp date�����t�Aint days�������y�x�����Apre��true�ꍇ�͎��O�m�F�@�����������f����
//    bool sale_judge_v(Timestamp date, int days, bool pre) const {
//		bool ret = false;
//		for(;;) {
//			// ���@�\�����@�����۔��f
//			if(!td_.v_issue_permit_) break;
//			// �������ԃ`�F�b�N 
//			// �����J�n���͉^�p���Ŕ�r
//			OpDateTime datetime(date);
//			// ������^�p���t�@3:00
//            Timestamp date_op = datetime.season_system();
//			if(date_op < td_.issue_start_date_) break;
//			// �����I�����͎����t�Ŕ�r
//			Date date_true(date);
//			Date end_date_true(td_.issue_end_date_);
//			if(!td_.is_indefinite_ && date_true > end_date_true) break;
//
//			// �y�x�����f
//			// �����@�L�������͓����̂�
//			if(td_.is_holiday_ && days != ExtraTicketAccount::HOLIDAY) break;
//			// pre��true�ꍇ�͎��O�m�F�@�����������f����
//			if(!pre) {
//				// ��挔������������i�x���j
//				int s_time = 0;
//				int e_time = 0;
//				int c_time = static_cast<int>(date.hour()*10000 + date.minute()*100 + date.second());
//				if(days == ExtraTicketAccount::HOLIDAY) {
//					s_time = td_.limit_holiday_set_start_hour_*10000 + td_.limit_holiday_set_start_minute_*100;
//					e_time = td_.limit_holiday_set_end_hour_*10000   + td_.limit_holiday_set_end_minute_*100;
//					// ��挔������������i�����j
//				} else if (days == ExtraTicketAccount::WEEKDAY) {
//					s_time = td_.limit_weekday_set_start_hour_*10000 + td_.limit_weekday_set_start_minute_*100;
//					e_time = td_.limit_weekday_set_end_hour_*10000   + td_.limit_weekday_set_end_minute_*100;
//				} else { 
//					THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"days error!!",__FILE__,__LINE__); 
//				}
//				OMRONSOFT_TRACE("��Ԑ����J�n���� = " << s_time << "  ��Ԑ����I������ = " << e_time << "  ���ݎ��� = " << c_time)
//					int vres = e_time - s_time; if(vres < 0) vres += 240000;
//				int vcur = c_time - s_time; if(vcur < 0) vcur += 240000;
//				if(vres > vcur) break;                                               // ��Ԑ������ԓ�
//			}
//			ret = true;
//			break;
//		}
//		return ret;
//	}
//
	//�����\�`�F�b�N�i�����p�j true:�����Afalse:�����s��
	// INPUT:Timestamp date�����t�Aint days�������y�x�����Apre��true�ꍇ�͎��O�m�F�@�����������f����
    bool sale_judge_cm(Timestamp date, int days, bool pre) const {
		bool ret = false;
		for(;;) {
			// ���������۔��f
			if(!td_.cm_issue_permit_) break;
			// �O���肠��
			if(td_.is_pre_issue_) {
				// �O����`�F�b�N
				// �����J�n���@���@�O����J�n��
				// �O�����͉^�p���Ŕ�r
				// ������^�p���t�@3:00
				OpDateTime datetime_pre(date);
				Timestamp date_op_pre = datetime_pre.season_system();
				if(date_op_pre < td_.pre_issue_start_date_) break;
			} else {
				// �������ԃ`�F�b�N 
				// �����J�n���͉^�p���Ŕ�r
				// ������^�p���t�@3:00
				OpDateTime datetime(date);
				Timestamp date_op = datetime.season_system();
				if(date_op < td_.issue_start_date_) break;
			}
			// �����I�����͎����t�Ŕ�r
			Date date_true(date);
			Date end_date_true(td_.issue_end_date_);
			if(!td_.is_indefinite_ && date_true > end_date_true) break;

			// �L���������P�̏ꍇ�A����ёO���茔�ł͂Ȃ��@�y�x�����f
			//if(td_.cm_valid_date_count_ == 1 && !td_.is_pre_issue_ && !pre) {
			if(!pre) {
				if(td_.is_holiday_ && days != ExtraTicketAccount::HOLIDAY) break;
			}

			// pre��true�ꍇ�͎��O�m�F�@�����������f����
			if(!pre) {
				// ��挔������������i�x���j
				int s_time = 0;
				int e_time = 0;
				int c_time = static_cast<int>(date.hour()*10000 + date.minute()*100 + date.second());
				if(days == ExtraTicketAccount::HOLIDAY) {
					s_time = td_.limit_holiday_set_start_hour_*10000 + td_.limit_holiday_set_start_minute_*100;
					e_time = td_.limit_holiday_set_end_hour_*10000   + td_.limit_holiday_set_end_minute_*100;
					// ��挔������������i�����j
				} else if (days == ExtraTicketAccount::WEEKDAY) {
					s_time = td_.limit_weekday_set_start_hour_*10000 + td_.limit_weekday_set_start_minute_*100;
					e_time = td_.limit_weekday_set_end_hour_*10000   + td_.limit_weekday_set_end_minute_*100;
				} else { 
					THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"days error!!",__FILE__,__LINE__); 
				}
				OMRONSOFT_TRACE("��Ԑ����J�n���� = " << s_time << "  ��Ԑ����I������ = " << e_time << "  ���ݎ��� = " << c_time)
					int vres = e_time - s_time; if(vres < 0) vres += 240000;
				int vcur = c_time - s_time; if(vcur < 0) vcur += 240000;
				if(vres > vcur) break;                                               // ��Ԑ������ԓ�
			}
			ret = true;
			break;
		}
		return ret;
	}

    //��挔����
    const char* extra_ticket_name() const { return tfd_.extra_ticket_name_.c_str(); }

	//�����{�^�������@�a��1�i��
	const char* v_button_word_jp_1() const { return tfd_.v_button_word_jp_1_.c_str(); }

	//�����{�^�������@�a��2�i��
	const char* v_button_word_jp_2() const { return tfd_.v_button_word_jp_2_.c_str(); }

	//�����{�^�������@�p��1�i��
	const char* v_button_word_en_1() const { return tfd_.v_button_word_en_1_.c_str(); }

	//�����{�^�������@�p��2�i��
	const char* v_button_word_en_2() const { return tfd_.v_button_word_en_2_.c_str(); }

	//�����{�^�������a��1�i��
	const char* cm_button_word_jp_1() const { return tfd_.cm_button_word_jp_1_.c_str(); }

	//�����{�^�������a��2�i��
	const char* cm_button_word_jp_2() const { return tfd_.cm_button_word_jp_2_.c_str(); }


	//����in���ځj
	unsigned short ticket_kind(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].ticket_kind; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���ʃt�@�C�����O�in���ځj
	const char* ticketface_name(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].ticketface_name.c_str(); }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���z�󎚗L���in���ځj
	bool is_amount_print(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].is_amount_print; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���w�in���ځj
	void start_station(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].start_station.line;
			station = tfd_.issue_info_[count - 1].start_station.station; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�i���o�����O�󎚗L���in���ځj
	bool is_numbering_print(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].is_numbering_print; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�����{�^���\������
	unsigned short button_index() const { return td_.button_index_; }

	//�x�[�X���ʃR�[�h�i�`�`�q�j�Ŕ��f���銄���L���in���ځj
	bool discount_by_format_code(unsigned short count) const {
		bool ret = false;
		std::string format_code = "";
		format_code = tfd_.issue_info_[count - 1].format_code;
		if( (format_code == string("�a")) ||
			(format_code == string("�c")) ||
			(format_code == string("�e")) ||
			(format_code == string("�g")) ||
			(format_code == string("�i")) ||
			(format_code == string("�k")) ){
				ret = true;
		}
		return ret;
	}

	//------------��^���G���R�[�h�p------------
	//����R�i��^���j�in���ځj
	unsigned short fmbigst_ticket_kind3(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_ticket_kind3; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���w�P�i��^���j�in���ځj
	void fmbigst_start_station1(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_start_station1.line;
			station = tfd_.issue_info_[count - 1].fmbigst_start_station1.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���w�Q�i��^���j�in���ځj
	void fmbigst_start_station2(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_start_station2.line;
			station = tfd_.issue_info_[count - 1].fmbigst_start_station2.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���w�R�i��^���j�in���ځj
	void fmbigst_start_station3(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_start_station3.line;
			station = tfd_.issue_info_[count - 1].fmbigst_start_station3.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���w�P�i��^���j�in���ځj
	void fmbigst_arrive_station1(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_arrive_station1.line;
			station = tfd_.issue_info_[count - 1].fmbigst_arrive_station1.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���w�Q�i��^���j�in���ځj
	void fmbigst_arrive_station2(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_arrive_station2.line;
			station = tfd_.issue_info_[count - 1].fmbigst_arrive_station2.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���w�R�i��^���j�in���ځj
	void fmbigst_arrive_station3(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_arrive_station3.line;
			station = tfd_.issue_info_[count - 1].fmbigst_arrive_station3.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�P�i��^���j�in���ځj
	void fmbigst_via_station1(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station1.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station1.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�Q�i��^���j�in���ځj
	void fmbigst_via_station2(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station2.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station2.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�R�i��^���j�in���ځj
	void fmbigst_via_station3(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station3.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station3.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�S�i��^���j�in���ځj
	void fmbigst_via_station4(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station4.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station4.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�T�i��^���j�in���ځj
	void fmbigst_via_station5(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station5.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station5.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�U�i��^���j�in���ځj
	void fmbigst_via_station6(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station6.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station6.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�V�i��^���j�in���ځj
	void fmbigst_via_station7(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station7.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station7.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�W�i��^���j�in���ځj
	void fmbigst_via_station8(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station8.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station8.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�X�i��^���j�in���ځj
	void fmbigst_via_station9(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station9.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station9.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�P�O�i��^���j�in���ځj
	void fmbigst_via_station10(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_via_station10.line;
			station = tfd_.issue_info_[count - 1].fmbigst_via_station10.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�t���[��ԃR�[�h�i��^���j�in���ځj
	void fmbigst_free_area_code(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmbigst_free_area_code.line;
			station = tfd_.issue_info_[count - 1].fmbigst_free_area_code.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//����S�ڍׁi��^���j�in���ځj
	unsigned short fmbigst_ticket_kind4_detail(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_ticket_kind4_detail; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�������i��^���j�in���ځj
	unsigned char fmbigst_roundtrip_info(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_roundtrip_info; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�����E�y�x�i��^���j�in���ځj
	unsigned char fmbigst_ticket_type(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_ticket_type; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���i�R�[�h�i��^���j�in���ځj
	unsigned long fmbigst_item_code(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_item_code; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�����R�[�h�i��^���j�in���ځj
	unsigned char fmbigst_discount_code(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmbigst_discount_code; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//------------������G���R�[�h�p------------
	//����R�i������j�in���ځj
	unsigned short fmpass_ticket_kind3(unsigned short count) const {
		if(count > 0 && count <= ISSUE_NUM){ return tfd_.issue_info_[count - 1].fmpass_ticket_kind3; }
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���w�i������j�in���ځj
	void fmpass_start_station(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_start_station.line;
			station = tfd_.issue_info_[count - 1].fmpass_start_station.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//���w�i������j�in���ځj
	void fmpass_arrive_station(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_arrive_station.line;
			station = tfd_.issue_info_[count - 1].fmpass_arrive_station.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�P�i������j�in���ځj
	void fmpass_via_station1(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station1.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station1.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�Q�i������j�in���ځj
	void fmpass_via_station2(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station2.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station2.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�R�i������j�in���ځj
	void fmpass_via_station3(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station3.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station3.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�S�i������j�in���ځj
	void fmpass_via_station4(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station4.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station4.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�T�i������j�in���ځj
	void fmpass_via_station5(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station5.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station5.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�U�i������j�in���ځj
	void fmpass_via_station6(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station6.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station6.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�V�i������j�in���ځj
	void fmpass_via_station7(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station7.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station7.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�o�R�W�i������j�in���ځj
	void fmpass_via_station8(unsigned short count, unsigned char& line, unsigned char& station) const {
		if(count > 0 && count <= ISSUE_NUM){ 
			line    = tfd_.issue_info_[count - 1].fmpass_via_station8.line;
			station = tfd_.issue_info_[count - 1].fmpass_via_station8.station;}
		else{ THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"count error!!",__FILE__,__LINE__); }
	}

	//�Q�d���i������j�in���ځj
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


	////�����@�����ł���������擾
	//unsigned short account_number_v(Timestamp date, int days, bool collaboration, bool pre) const{
	//	unsigned short account_n = 0;
	//	for(int k = 0; k < ACCOUNT_NUM; ++k){
	//		unsigned short extra_ticket_no = 0;
	//		if(collaboration) {
	//		  extra_ticket_no = ecd_.button_display_order_sp_[k];
	//		} else {
	//		  extra_ticket_no = ecd_.button_display_order_[k];
	//		}
	//		// ��挔�}�X�^���Ɍ���
	//		for(int i = 0; i < ecd_.master_record_count_; ++i) {
	//			if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_) {
	//				// ��挔�t�H�[�}�b�g�f�[�^���Ɍ���
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

	//���������ł���������擾
	unsigned short account_number_cm(Timestamp date, int days, bool collaboration, bool pre) const{
		unsigned short account_n = 0;
		for(int k = 0; k < ACCOUNT_NUM; ++k){
			unsigned short extra_ticket_no = 0;
			if(collaboration) {
			  extra_ticket_no = ecd_.button_display_order_sp_[k];
			} else {
			  extra_ticket_no = ecd_.button_display_order_[k];
			}
			// ��挔�}�X�^���Ɍ���
			for(int i = 0; i < ecd_.master_record_count_; ++i) {
				if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_) {
					// ��挔�t�H�[�}�b�g�f�[�^���Ɍ���
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


	//�������擾�i�����j
	bool next_one(ExtraTicketAccount& eta, bool collaboration) {
		bool ret = false;
		if(collaboration) {
		  for(;next_cnt_sp_ >= 0 && next_cnt_sp_ < ACCOUNT_NUM;){
			unsigned short extra_ticket_no = ecd_.button_display_order_sp_[next_cnt_sp_];
			// ��挔�}�X�^���Ɍ���
			for(int i = 0; i < ecd_.master_record_count_; ++i) {
			  if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_) {
				// ��挔�t�H�[�}�b�g�f�[�^���Ɍ���
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
			// ��挔�}�X�^���Ɍ���
			for(int i = 0; i < ecd_.master_record_count_; ++i) {
			  if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_) {
				// ��挔�t�H�[�}�b�g�f�[�^���Ɍ���
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
  //�������擾�i��挔No�w��j
  bool get_account_ticket_no(unsigned short extra_ticket_no, ExtraTicketAccount& eta, bool collaboration) const{
    bool ret = false;
    // �����{�^���\���������Ɍ���
    for(int k = 0; k < ACCOUNT_NUM; ++k){
      unsigned short button_extra_ticket_no = 0;
      if(collaboration) {
        button_extra_ticket_no = ecd_.button_display_order_sp_[k];
      } else {
        button_extra_ticket_no = ecd_.button_display_order_[k];
      }
      if(extra_ticket_no == button_extra_ticket_no){
        // ��挔�}�X�^���Ɍ���
        for(int i = 0; i < ecd_.master_record_count_; ++i){
          if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_){
            // ��挔�t�H�[�}�b�g�f�[�^���Ɍ���
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
  //�������擾�i��挔No�w��@�������ߗp�j
  bool get_account_ticket_no_refund_cm(unsigned short extra_ticket_no, ExtraTicketAccount& eta) const{
    bool ret = false;
    // ��挔�}�X�^���Ɍ���
    for(int i = 0; i < ecd_.master_record_count_; ++i){
      if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_){
        // ��挔�t�H�[�}�b�g�f�[�^���Ɍ���
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

  ////��挔�\���ݒ�i�E�j(�����@�p)
  //bool get_right_button(ExtraTicketAccount& eta, bool collaboration) const{
  //  bool ret = false;
  //  unsigned short extra_ticket_no = 0;
  //  if(collaboration) {
  //    extra_ticket_no = ecd_.right_display_sp_;
  //  } else {
  //    extra_ticket_no = ecd_.right_display_;
  //  }
  //  // �����{�^���\���������Ɍ���
  //  for(int k = 0; k < ACCOUNT_NUM; ++k){
  //    unsigned short button_extra_ticket_no = 0;
  //    if(collaboration) {
  //      button_extra_ticket_no = ecd_.button_display_order_sp_[k];
  //    } else {
  //      button_extra_ticket_no = ecd_.button_display_order_[k];
  //    }
  //    if(extra_ticket_no == button_extra_ticket_no){
  //      // ��挔�}�X�^���Ɍ���
  //      for(int i = 0; i < ecd_.master_record_count_; ++i){
  //        if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_){
  //          // ��挔�t�H�[�}�b�g�f�[�^���Ɍ���
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
  ////��挔�\���ݒ�i���j(�����@�p)
  //bool get_left_button(ExtraTicketAccount& eta, bool collaboration) const{
  //  bool ret = false;
  //  unsigned short extra_ticket_no = 0;
  //  if(collaboration) {
  //    extra_ticket_no = ecd_.left_display_sp_;
  //  } else {
  //    extra_ticket_no = ecd_.left_display_;
  //  }
  //  // �����{�^���\���������Ɍ���
  //  for(int k = 0; k < ACCOUNT_NUM; ++k){
  //    unsigned short button_extra_ticket_no = 0;
  //    if(collaboration) {
  //      button_extra_ticket_no = ecd_.button_display_order_sp_[k];
  //    } else {
  //      button_extra_ticket_no = ecd_.button_display_order_[k];
  //    }
  //    if(extra_ticket_no == button_extra_ticket_no){
  //      // ��挔�}�X�^���Ɍ���
  //      for(int i = 0; i < ecd_.master_record_count_; ++i){
  //        if(extra_ticket_no == eetd_.master_.ticket_data_[i].extra_ticket_no_){
  //          // ��挔�t�H�[�}�b�g�f�[�^���Ɍ���
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

	//�}�X�^���擾
	const char* name() const { return MasterDef::EXTRA_TICKET; }

	//�}�X�^�t�@�C�����擾
	const char* getMasterFileName() const {
		return FILE_PATH;
	}

	//�ݒ�o�[�W�����擾
	const char* version() const {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::version() > ");
		const char* ret = MasterDef::UNKNOWN;
//		if(data_.size()){ ret = etd_.prefix_.st_set_version_.c_str(); }
		OMRONSOFT_TRACE("ExtraTicketMasterRep::version() < ");
		return ret;
	}

	////�}�X�^�o�[�W�����擾
	//const char* master_version() const {
	//	OMRONSOFT_TRACE("ExtraTicketMasterRep::master_version() > ");
	//	const char* ret = MasterDef::UNKNOWN;
	//	if(data_.size()){ ret = etd_.prefix_.st_master_version_.c_str(); }
	//	OMRONSOFT_TRACE("ExtraTicketMasterRep::master_version() < ");
	//	return ret;
	//}

	//��挔�f�[�^�o�[�W�����擾
	const char* master_version() const {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::master_version() > ");
		const char* ret = MasterDef::UNKNOWN;
		if(data_.size()){
			// �V�X�e�����Ԃɂ��A�L���Ȋ�挔�}�X�^�擾
			OpDateTime opDate(Timestamp::current());

			Timestamp com_date_op = opDate.auto_farechange();
			OMRONSOFT_TRACE("������t����:" << com_date_op.year() << "�N" <<
			com_date_op.month() << "��" << com_date_op.day() << "��");
			
			int ver(0);
			if(master_no(com_date_op,ver)) {
				ret = etd_.comprefix_.comdata_[ver].st_data_version_.c_str();
			}
		}
		OMRONSOFT_TRACE("ExtraTicketMasterRep::master_version() < ");
		return ret;
	}

	//��挔�f�[�^(�z�M�f�[�^)�쐬���擾
	int master_data_delivery_time(Timestamp& delivery_time) const {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::master_data_delivery_time() > ");
		int ret = ExtraTicketMaster::NG;
		if(data_.size()){ 
			delivery_time = etd_.comprefix_.data_delivery_time_;
			ret = ExtraTicketMaster::OK;
		}else{
			//�����f�[�^�ُ�
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}
		OMRONSOFT_TRACE("ExtraTicketMasterRep::master_data_delivery_time() < ");
		return ret;
	}


	////�T���l�擾
	//unsigned long sum() const {
	//	OMRONSOFT_TRACE("ExtraTicketMasterRep::sum() > ");
	//	unsigned long ret = 0;

	//	if(data_.size()) {
	//		ret = etd_.prefix_.sum_;
	//	}

	//	OMRONSOFT_TRACE("ExtraTicketMasterRep::sum() < ");
	//	return ret;
	//}

	//�T���l�v�Z
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

	//�}�X�^�f�[�^�Z�b�g
	int data_set(const char* data, long size){
		OMRONSOFT_TRACE("ExtraTicketMasterRep::data_set() > ");
		int ret = ExtraTicketMaster::NG;
		// data_size�͎g��Ȃ��B
		long data_size = 0;
		int error_detail = ERROR_OTHER;
		string buff_data;
		data_ = "";
		if(etd_.internalize(data, data_size, error_detail)){
			string dat1_path = ConstVal::instance().dat1_path();// DAT1�p�X�擾
			string dat2_path = ConstVal::instance().dat2_path();// DAT2�p�X�擾
			DoubleData dd(true, dat1_path.c_str(), dat2_path.c_str());// ��d�f�[�^�Ǘ��N���X
			string file_path = dat1_path + string(FILE_PATH);   // ��΃p�X�쐬
//			buff_data = string(data,data_size); // �ʐM���w�b�_���Ȃ��̂ŁA�f�[�^�T�C�Y�擾�ł��܂���
			buff_data = string(data,size);

			// ��d���p�f�[�^�Z�b�g
			DD_RES eRes = dd.DoubleSetData(file_path.c_str(), buff_data.data(),
			                               buff_data.size(), ios::binary );

			if(eRes == DD_RES_OK) {
				// �f�[�^�Z�b�g����
				data_ = buff_data;
				ret = ExtraTicketMaster::OK;
			} else {
				// �f�[�^�Z�b�g���s
				// �ُ�ڍׂ��擾
				int iDat1Err = dd.GetErrDetail(true);
				int iDat2Err = dd.GetErrDetail(false);
				// �ُ��ʒm����
				if(iDat1Err) {
					NOTICE_DBLDATA_ERROR(iDat1Err," DataSet Error ",file_path);
				}
				if(iDat2Err) {
					NOTICE_DBLDATA_ERROR(iDat2Err," DataSet Error ",dd.CvtDat1ToDat2Path(file_path.c_str()));
				}
				THROW_ERROR(MasterDef::WRITE_ERR,"Data Format Error",__FILE__,__LINE__)
			}
		}else{
			//�f�[�^�t�H�[�}�b�g�G���[
			OMRONSOFT_TRACE("Data Format Error");
			THROW_ERROR(MasterDef::FUNC_PARAM_ERR,"Data Format Error",__FILE__,__LINE__)
		}
		OMRONSOFT_TRACE("ExtraTicketMaster::data_set() < ");
		return ret;
	}

	//�}�X�^�f�[�^�擾
	int data_get(const char*& data, long& size) const{
		OMRONSOFT_TRACE("ExtraTicketMasterRep::data_get() > ");
		int ret = ExtraTicketMaster::NG;
		if(data_.size()){
			data = data_.data();
			size = data_.size();
			ret = ExtraTicketMaster::OK;
		}else{
			//�����f�[�^�ُ�
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}

		OMRONSOFT_TRACE("ExtraTicketMasterRep::data_get() < ");
		return ret;
	}

	//�L���J�n�N���������擾�i�}�X�^����master_id�Ԗڂ̃f�[�^�̗L���J�n�����擾�j
	int valid_date(int master_id, Timestamp& valid_date) const {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::valid_date() > ");
		int ret = ExtraTicketMaster::NG;

		if(data_.size()){
			if(master_id > 0 && master_id <= MASTER_NUM){
				valid_date = etd_.comprefix_.comdata_[master_id - 1].valid_date_;
				ret = ExtraTicketMaster::OK;
			}
		}else{
			//�����f�[�^�ُ�
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}

		OMRONSOFT_TRACE("ExtraTicketMasterRep::valid_date() < ");
		return ret;
	}

	//�L���J�n�N������������уo�[�W�����擾�i�}�X�^����master_id�Ԗڂ̃f�[�^�̗L���J�n������уo�[�W�������擾�j
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
			//�����f�[�^�ُ�
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}

		OMRONSOFT_TRACE("ExtraTicketMasterRep::valid_date() < ");
		return ret;
	}

	//�L���}�X�^�擾
	int get_master(const Timestamp& real_date, EachExtraTicketMaster& valid_master) {
		OMRONSOFT_TRACE("ExtraTicketMasterRep::get_master() > ");
		int ret = ExtraTicketMaster::NG;
		int new_id = 0;
		int old_id = 0;
		Timestamp real_date_tmp(real_date);
		OpDateTime com_date(real_date_tmp);
		Timestamp com_date_op = com_date.auto_farechange();
		OMRONSOFT_TRACE("������t����:" << com_date_op.year() << "�N" <<
			com_date_op.month() << "��" << com_date_op.day() << "��");
		if(data_.size()){
			//�}�X�^�P�C�Q�̂����ǂ��炪�V�������𒲂ׂ�
			//�������͂P��V�����}�X�^�Ƃ���
			if(etd_.comprefix_.comdata_[0].valid_date_ >= etd_.comprefix_.comdata_[1].valid_date_){
				new_id = 0; old_id = 1;
			}else{
				new_id = 1; old_id = 0;
			}
			OMRONSOFT_TRACE("�V�}�X�^�F�}�X�^" << new_id + 1 <<
				"�A���}�X�^�F�}�X�^" << old_id + 1);

			OMRONSOFT_TRACE("�}�X�^�P���t����:" <<
				etd_.comprefix_.comdata_[0].valid_date_.year() << "�N" <<
				etd_.comprefix_.comdata_[0].valid_date_.month() << "��" <<
				etd_.comprefix_.comdata_[0].valid_date_.day() << "��" <<
				etd_.comprefix_.comdata_[0].valid_date_.hour() << "��" <<
				etd_.comprefix_.comdata_[0].valid_date_.minute() << "��" <<
				etd_.comprefix_.comdata_[0].valid_date_.second() << "�b");
			OMRONSOFT_TRACE("�}�X�^�Q���t����:" <<
				etd_.comprefix_.comdata_[1].valid_date_.year() << "�N" <<
				etd_.comprefix_.comdata_[1].valid_date_.month() << "��" <<
				etd_.comprefix_.comdata_[1].valid_date_.day() << "��" <<
				etd_.comprefix_.comdata_[1].valid_date_.hour() << "��" <<
				etd_.comprefix_.comdata_[1].valid_date_.minute() << "��" <<
				etd_.comprefix_.comdata_[1].valid_date_.second() << "�b");
			//�}�X�^�P�C�Q�̂����ǂ��炪�L�����𒲂ׂ�
			//�܂��A�V����������
			//�V�������łȂ���ΌÂ��ق��𒲂ׂ�
			if(com_date_op >= etd_.comprefix_.comdata_[new_id].valid_date_){
				OMRONSOFT_TRACE("�L���}�X�^����F�}�X�^" << new_id + 1);
				valid_master.rep_->eetd_ = etd_.eachdata_[new_id];
				valid_master.rep_->ecd_ = etd_.comprefix_.comdata_[new_id];
				update_kind_map(true);
				ret = ExtraTicketMaster::OK;
			}else if(com_date_op >= etd_.comprefix_.comdata_[old_id].valid_date_){
				OMRONSOFT_TRACE("�L���}�X�^����F�}�X�^" << old_id + 1);
				valid_master.rep_->eetd_ = etd_.eachdata_[old_id];
				valid_master.rep_->ecd_ = etd_.comprefix_.comdata_[old_id];
				update_kind_map(true);
				ret = ExtraTicketMaster::OK;
			}

		}else{
			//�����f�[�^�ُ�
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
			//�}�X�^�P�C�Q�̂����ǂ��炪�V�������𒲂ׂ�
			//�������͂P��V�����}�X�^�Ƃ���
			if(etd_.comprefix_.comdata_[0].valid_date_ >= etd_.comprefix_.comdata_[1].valid_date_){
				new_id = 0; old_id = 1;
			}else{
				new_id = 1; old_id = 0;
			}
			OMRONSOFT_TRACE("�V�}�X�^�F�}�X�^" << new_id + 1 <<
				"�A���}�X�^�F�}�X�^" << old_id + 1);

			//�}�X�^�P�C�Q�̂����ǂ��炪�L�����𒲂ׂ�
			//�܂��A�V����������
			//�V�������łȂ���ΌÂ��ق��𒲂ׂ�
			if(com_date_op >= etd_.comprefix_.comdata_[new_id].valid_date_){
				OMRONSOFT_TRACE("�L���}�X�^����F�}�X�^" << new_id + 1);
				valid_master.rep_->eetd_ = etd_.eachdata_[new_id];
				valid_master.rep_->ecd_ = etd_.comprefix_.comdata_[new_id];
				cur_id = new_id + 1;
			}else if(com_date_op >= etd_.comprefix_.comdata_[old_id].valid_date_){
				OMRONSOFT_TRACE("�L���}�X�^����F�}�X�^" << old_id + 1);
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
			//�����f�[�^�ُ�
			OMRONSOFT_TRACE("Data Error");
			//THROW_ERROR(MasterDef::PARAM_ERR,"Data Error",__FILE__,__LINE__);
		}
		return ret;
	}


	//�ݒu�w�R�[�h�擾
	unsigned long station_setting_code() const{
		OMRONSOFT_TRACE("ExtraTicketMasterRep::station_setting_code() > ");
		unsigned long ret = ExtraTicketMaster::NG;
		if(data_.size()){ ret = etd_.comprefix_.station_setting_code_; }
		OMRONSOFT_TRACE("ExtraTicketMasterRep::station_setting_code() < ");
		return ret;
	}



	//�f�o�b�O�p
	//    void print() {
	//      ExtraTicketMasterData etmd;
	//      if(etmd.internalize(data_.data(),data_.size())){
	//        cout << "[�w�b�_]" << std::endl;
	//        cout << " �ݒ��ް�ޮ� Ͻ��ް�ޮ�  �ް�����    �T���l" << std::endl;
	//        cout << setw(11) << etmd.prefix_.st_version_;
	//        cout << setw(10) << (unsigned int)etmd.prefix_.master_version_;
	//        cout << setw(10) << etmd.prefix_.size_;
	//        cout << setw(11) << etmd.prefix_.sum_ << std::endl;;

	//        for(int i = 0; i < MASTER_NUM; ++i){
	//          EachMasterData e;
	//          e = etmd.master_[i];
	//          cout << "[�}�X�^" << i+1 << "]  �L���J�n�N��������:"
	//               << e.valid_date_.year() << "/" << e.valid_date_.month() << "/"
	//               << e.valid_date_.day()  << " " << e.valid_date_.hour()  << ":"
	//               << e.valid_date_.minute() << std::endl;
	//          cout << "����" << "                             " << "                 " << "                 " << "����"
	//               << "  ����    "<< "  ����    "<< " ���O "<< " �L�� "<< "   �L��   "<< " ���� "<< " ���v "<< " ���v "
	//               << "�� " << " ��l" << " ����" << " ���C" << "  ���w  " << "  ���w  " << "  ���w  " << "����"
	//               << " ����" << " ��" << " �^" << " �A" << " ��" << " ��" << " ��" << " ��" << "  �ذ" << " �r"
	//               << " �D" << " ���w" << " ���w" << " ���w" << " ���w" << "  �o�R  " << "  �o�R  " << "  �o�R  "
	//               << "  �o�R  " << "  �o�R  " << "  �o�R  " << "  �o�R  " << "  �o�R  " << "  �o�R  " << "  �o�R  "
	//               << "  ���w  " << "  ���w  " << "  ���w  " << "�ذ " <<std::endl;
	//          cout << "�ԍ�" << "             ������          " << "  �����́i��i�j " << "  �����́i���i�j " << "�ʒu"
	//               << "  �J�n��  "<< "  �I����  "<< " ���� "<< " ���� "<< "   �I���� "<< " ���� "<< " ��l "<< " ���� "
	//               << "�����" << " ���z" << " ���z" << "   ��" << "  (�P)  " << "  (�Q)  " << "  (�R)  " << "  �R"
	//               << " �\��" << " ��" << " ��" << " ��" << " ��" << " ��" << " ��" << " ��" << " �ޯ�" << " ��"
	//               << " ��" << " 1-2 " << " �ر " << " 1-2 " << " �ر " << "  (�P)  " << "  (�Q)  " << "  (�R)  "
	//               << "  (�S)  " << "  (�T)  " << "  (�U)  " << "  (�V)  " << "  (�W)  " << "  (�X)  " << "  (10)  "
	//               << "  (�P)  " << "  (�Q)  " << "  (�R)  " << "���" <<std::endl;

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
	// FUNCTION   : ExtraTicketMaster�f�[�^�`�F�b�N����
	// PARAM      : �Ȃ�
	// RETURN     : �Ȃ�
	//************************************************************************************
	void CheckData() {
		OMRONSOFT_TRACE("ExtraTicketMaster::CheckData() start ");
		string dat1_path = ConstVal::instance().dat1_path();    // DAT1�p�X�擾
		string dat2_path = ConstVal::instance().dat2_path();    // DAT2�p�X�擾
		DoubleData dd(true, dat1_path.c_str(), dat2_path.c_str());  // ��d�f�[�^�Ǘ��N���X
		string file_path = dat1_path + string(FILE_PATH);   // ��΃p�X�쐬
		// ��d�Ǘ��p�f�B���N�g���`�F�b�N���쐬
		DD_RES eRes = dd.DoubleMakeDirectory(file_path.c_str(), true);
		if( eRes != DD_RES_OK ) {
			// �ُ�ڍׂ��擾
			int iDat1Err = dd.GetErrDetail(true);
			int iDat2Err = dd.GetErrDetail(false);
			// �ُ킪����Βʒm����
			if(iDat1Err) {
				NOTICE_DBLDATA_ERROR(iDat1Err," MakeDir Error ",file_path);
			}
			if(iDat2Err) {
				NOTICE_DBLDATA_ERROR(iDat2Err," MakeDir Error ",dd.CvtDat1ToDat2Path(file_path.c_str()));
			}
		}

		// ��d������E����
		eRes = dd.MatchingData(file_path.c_str());
		// �ُ�ڍׂ��擾
		int iDat1Err = dd.GetErrDetail(true);
		int iDat2Err = dd.GetErrDetail(false);
		switch(eRes) {
			case DD_RES_OK :              // ����I��
				// �������Ȃ�
				break;
			case DD_RES_RECOVERY_MAIN :   // ��f�[�^����
			case DD_RES_RECOVERY_SUB  :   // �]�f�[�^����
				// �ُ��o�^����i�o�^�̂ݕ\���͂��Ȃ��j
				if(iDat1Err) {
					COUNT_DBLDATA_ERROR(iDat1Err,"DataRecover : ", file_path);
				}
				if(iDat2Err) {
					COUNT_DBLDATA_ERROR(iDat2Err,"DataRecover : ", dd.CvtDat1ToDat2Path(file_path.c_str()));
				}
				break;
			case DD_RES_MAIN_NG :         // ��f�[�^�t�@�C���ُ�
			case DD_RES_SUB_NG  :         // �]�f�[�^�t�@�C���ُ�
			case DD_RES_MAIN_SUB_NG :     // �偕�]�f�[�^�t�@�C���ُ�
			case DD_RES_SUM_NG  :         // �T���l�s��v�ُ�
			case DD_RES_NG :              // ���̑��ُ�(������NULL��)
			default :
				// �ُ��ʒm����
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

	// �L���}�X�^�ԍ��擾�i�O�F�V�@�P�F���j
	bool master_no(const Timestamp& date, int& no) const {
		bool ret(false);
		int new_id = 0;
		int old_id = 0;
		//�}�X�^�P�C�Q�̂����ǂ��炪�V�������𒲂ׂ�
		//�������͂P��V�����}�X�^�Ƃ���
		if(etd_.comprefix_.comdata_[0].valid_date_ >= etd_.comprefix_.comdata_[1].valid_date_){
			new_id = 0; old_id = 1;
		}else{
			new_id = 1; old_id = 0;
		}
		//�}�X�^�P�C�Q�̂����ǂ��炪�L�����𒲂ׂ�
		//�܂��A�V����������
		//�V�������łȂ���ΌÂ��ق��𒲂ׂ�
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

		//Master�f�[�^�`�F�b�N
		CheckData();

		string dat1_path = ConstVal::instance().dat1_path();// DAT1�p�X�擾
        string file_path = dat1_path + string(FILE_PATH);   // ��΃p�X�쐬
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
				OMRONSOFT_TRACE("������ Format OK!! ������");
				data_ = buff_data;
			}else{
				msg = "File Format Error ";
				msg += FILE_PATH;
				OMRONSOFT_TRACE(msg.c_str());
				//�N�����̓t�@�C�����t�H�[�}�b�g�ُ�ł��ُ�ɂ��Ȃ�
				//INTERNAL_ERROR(0, ErrOccur::MTC_EXTRA_TICKET_MASTER_ERR, msg.c_str());
			}
		}else{
			msg ="File Open Error ";
			msg += FILE_PATH;
			OMRONSOFT_TRACE(msg.c_str());
			//�N�����̓t�@�C�����J���Ȃ��Ă��ُ�ɂ��Ȃ�
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

