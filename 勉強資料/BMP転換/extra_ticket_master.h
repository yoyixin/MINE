//
// Author  : Sai
// Created : Oct 2012
//
// extra_ticket_master.h

#ifndef _omronsoft_pub_extra_ticket_master_h_
#define _omronsoft_pub_extra_ticket_master_h_

#include <omronsoft/multiprops.h>
#include <omronsoft/datetime.h>
#include <omronsoft/sa1.h>
#include "../master.h"

namespace omronsoft {
namespace pub {
namespace extraticketmaster {

class ExtraTicketAccountRep;
class SA1_STANDARD_LIB_API ExtraTicketAccount {
public:

	enum DAYS {
		WEEKDAY   = 0,  //����
		HOLIDAY   = 1,  //�y�x��
	};

	enum PLANNINGKIND {
		PLANNING_UNKNOWN = 0,
		PLANNING_PASS = 2,        // �����
		PLANNING_BIG_ST = 3,      // ��^��
		PLANNING_NO_ENCODE = 7,   // �m�[�G���R�[�h��
	};

	//��挔No
	unsigned short extra_ticket_no() const;
	//�����f�[�^���No
	unsigned short issue_data_kind_no() const;
	//�����f�[�^No
	unsigned short issue_data_no() const;
	//���߃f�[�^���No
	unsigned short refund_data_kind_no() const;
	//���߃f�[�^No
	unsigned short refund_data_no() const;
	//�������z�i��l�j
	unsigned short issue_amount_adult() const;
	//�������z�i�����j
	unsigned short issue_amount_child() const;
	//n���ڕ\���^���i��l�j
	unsigned short ticket_fare_adult(unsigned short count) const;
	//n���ڕ\���^���i�����j
	unsigned short ticket_fare_child(unsigned short count) const;
	//���s����
	unsigned short issue_num() const;
	//�����J�n�N����
	Timestamp issue_start_date() const;
	//�����I���N����
	Timestamp issue_end_date() const;
	//�������ł��邩�ǂ���
	bool is_indefinite() const;
	//�O���J�n�N����
	Timestamp pre_issue_start_date() const;
	//�O�������邩�ǂ���
	bool is_pre_issue() const;
//	//���@�\�����@�p�L������
//	unsigned short v_valid_date_count() const;
	//�����p�L�������L������
	unsigned short cm_valid_date_count() const;
	//�C�x���g�Œ�L���J�n��
	Timestamp event_start_date() const;
	//�C�x���g�Œ�L���I����
	Timestamp event_end_date() const;
//	//���@�\�����@������
//	bool v_issue_permit() const;
//	//���@�\�����@���߉�
//	bool v_refund_permit() const;
	//���������@������
	bool cm_issue_permit() const;
	//���������@���߉�
	bool cm_refund_permit() const;
	//�y�x��
	bool is_holiday() const;
	//��挔�����ݒ�i�����j�J�n��
	unsigned short limit_weekday_set_start_hour() const;
	//��挔�����ݒ�i�����j�J�n��
	unsigned short limit_weekday_set_start_minute() const;
	//��挔�����ݒ�i�����j�I����
	unsigned short limit_weekday_set_end_hour() const;
	//��挔�����ݒ�i�����j�I����
	unsigned short limit_weekday_set_end_minute() const;
	//��挔�����ݒ�i�x���j�J�n��
	unsigned short limit_holiday_set_start_hour() const;
	//��挔�����ݒ�i�x���j�J�n��
	unsigned short limit_holiday_set_start_minute() const;
	//��挔�����ݒ�i�x���j�I����
	unsigned short limit_holiday_set_end_hour() const;
	//��挔�����ݒ�i�x���j�I����
	unsigned short limit_holiday_set_end_minute() const;
//    //�����@�����\�`�F�b�N true:�����Afalse:�����s�Apre��true�ꍇ�͎��O�m�F�@�����������f����
//    bool sale_judge_v(Timestamp date, int days, bool pre = false) const;
    //���������\�`�F�b�N true:�����Afalse:�����s�Apre��true�ꍇ�͎��O�m�F�@�����������f����
    bool sale_judge_cm(Timestamp date, int days, bool pre = false) const;
	//��挔����
    const char* extra_ticket_name() const;
	//�����{�^�������@�a��1�i��
	const char* v_button_word_jp_1() const;
	//�����{�^�������@�a��2�i��
	const char* v_button_word_jp_2() const;
	//�����{�^�������@�p��1�i��
	const char* v_button_word_en_1() const;
	//�����{�^�������@�p��2�i��
	const char* v_button_word_en_2() const;
	//�����{�^�������a��1�i��
	const char* cm_button_word_jp_1() const;
	//�����{�^�������a��2�i��
	const char* cm_button_word_jp_2() const;
	//����in���ځj
	unsigned short ticket_kind(unsigned short count) const;
	//���ʃt�@�C�����O�in���ځj
	const char* ticketface_name(unsigned short count) const;
	//���z�󎚗L���in���ځj
	bool is_amount_print(unsigned short count) const;
	//���w�in���ځj
	void start_station(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�i���o�����O�󎚗L���in���ځj
	bool is_numbering_print(unsigned short count) const;
	//�����{�^���\������
	unsigned short button_index() const;
	//�x�[�X���ʃR�[�h�i�`�`�q�j�Ŕ��f���銄���L��
	bool discount_by_format_code(unsigned short count) const;

	//------------��^���G���R�[�h�p------------
	//����R�i��^���j�in���ځj
	unsigned short fmbigst_ticket_kind3(unsigned short count) const;
	//���w�P�i��^���j�in���ځj
	void fmbigst_start_station1(unsigned short count, unsigned char& line, unsigned char& station) const;
	//���w�Q�i��^���j�in���ځj
	void fmbigst_start_station2(unsigned short count, unsigned char& line, unsigned char& station) const;
	//���w�R�i��^���j�in���ځj
	void fmbigst_start_station3(unsigned short count, unsigned char& line, unsigned char& station) const;
	//���w�P�i��^���j�in���ځj
	void fmbigst_arrive_station1(unsigned short count, unsigned char& line, unsigned char& station) const;
	//���w�Q�i��^���j�in���ځj
	void fmbigst_arrive_station2(unsigned short count, unsigned char& line, unsigned char& station) const;
	//���w�R�i��^���j�in���ځj
	void fmbigst_arrive_station3(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�P�i��^���j�in���ځj
	void fmbigst_via_station1(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�Q�i��^���j�in���ځj
	void fmbigst_via_station2(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�R�i��^���j�in���ځj
	void fmbigst_via_station3(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�S�i��^���j�in���ځj
	void fmbigst_via_station4(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�T�i��^���j�in���ځj
	void fmbigst_via_station5(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�U�i��^���j�in���ځj
	void fmbigst_via_station6(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�V�i��^���j�in���ځj
	void fmbigst_via_station7(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�W�i��^���j�in���ځj
	void fmbigst_via_station8(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�X�i��^���j�in���ځj
	void fmbigst_via_station9(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�P�O�i��^���j�in���ځj
	void fmbigst_via_station10(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�t���[��ԃR�[�h�i��^���j�in���ځj
	void  fmbigst_free_area_code(unsigned short count, unsigned char& line, unsigned char& station) const;
	//����S�ڍׁi��^���j�in���ځj
	unsigned short fmbigst_ticket_kind4_detail(unsigned short count) const;
	//�������i��^���j�in���ځj
	unsigned char fmbigst_roundtrip_info(unsigned short count) const;
	//�����E�y�x�i��^���j�in���ځj
	unsigned char fmbigst_ticket_type(unsigned short count) const;
	//���i�R�[�h�i��^���j�in���ځj
	unsigned long fmbigst_item_code(unsigned short count) const;
	//�����R�[�h�i��^���j�in���ځj
	unsigned char fmbigst_discount_code(unsigned short count) const;


	//------------������G���R�[�h�p------------
	//����R�i������j�in���ځj
	unsigned short fmpass_ticket_kind3(unsigned short count) const;
	//���w�i������j�in���ځj
	void fmpass_start_station(unsigned short count, unsigned char& line, unsigned char& station) const;
	//���w�i������j�in���ځj
	void fmpass_arrive_station(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�P�i������j�in���ځj
	void fmpass_via_station1(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�Q�i������j�in���ځj
	void fmpass_via_station2(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�R�i������j�in���ځj
	void fmpass_via_station3(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�S�i������j�in���ځj
	void fmpass_via_station4(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�T�i������j�in���ځj
	void fmpass_via_station5(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�U�i������j�in���ځj
	void fmpass_via_station6(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�V�i������j�in���ځj
	void fmpass_via_station7(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�o�R�W�i������j�in���ځj
	void fmpass_via_station8(unsigned short count, unsigned char& line, unsigned char& station) const;
	//�Q�d���i������j�in���ځj
	bool fmpass_doubleness_bit(unsigned short count) const;

	ExtraTicketAccount();
	ExtraTicketAccount(const ExtraTicketAccount&);
	ExtraTicketAccount& operator=(const ExtraTicketAccount&);
	virtual ~ExtraTicketAccount();

private:
	friend class EachExtraTicketMasterRep;
	ExtraTicketAccountRep* rep_;
};


class EachExtraTicketMasterRep;
class SA1_STANDARD_LIB_API EachExtraTicketMaster {
public:

	// ���А���挔�����������ɁAcollaboration��true�ɂ���

	//�����@�����ł���������擾
	unsigned short account_number_v(Timestamp date, int days, bool collaboration = false, bool pre = false) const;
	//���������ł���������擾
	unsigned short account_number_cm(Timestamp date, int days, bool collaboration = false, bool pre = false) const;
	//�������擾�i�����j
	bool next_one(ExtraTicketAccount& eta, bool collaboration = false);
	void first(bool collaboration = false);

	//�������擾�i��挔No�w��j
	bool get_account_ticket_no(unsigned short extra_ticket_no, ExtraTicketAccount& eta, bool collaboration = false) const;
	//�������擾�i��挔No�w��@�������ߗp�j
	bool get_account_ticket_no_refund_cm(unsigned short extra_ticket_no, ExtraTicketAccount& eta) const;

	//��挔�\���ݒ�i�E�j(�����@�p)
	bool get_right_button(ExtraTicketAccount& eta, bool collaboration = false) const;
	//��挔�\���ݒ�i���j(�����@�p)
	bool get_left_button(ExtraTicketAccount& eta, bool collaboration = false) const;

	EachExtraTicketMaster();
	EachExtraTicketMaster(const EachExtraTicketMaster&);
	EachExtraTicketMaster& operator=(const EachExtraTicketMaster&);
	virtual ~EachExtraTicketMaster();

private:
	friend class ExtraTicketMasterRep;
	EachExtraTicketMasterRep* rep_;
};


class ExtraTicketMasterRep;
class SA1_STANDARD_LIB_API ExtraTicketMaster : public omronsoft::pub::master::MasterData {
public:
	virtual ~ExtraTicketMaster();

	enum{NG,OK};  //data_set(),data_get()�̖߂�l

	enum MASTERID {
		NEW_VERSION = 1,
		CUR_VERSION = 2
	};

	enum ErrorDetail {
		ERROR_OK,
		ERROR_STATION,
		ERROR_OTHER,
	};

	struct PlanDataKind {
		unsigned short dataKindNo;
	    unsigned short dataNo;
	};

	typedef std::map<unsigned short, PlanDataKind> KindMap;
	typedef std::pair<unsigned short,PlanDataKind> KindElement;
	KindMap m_DataKindMap;

	int m_CurMasterID;

	//--------------------------------------------------------------------------
	//�}�X�^�f�[�^���擾 name()
	//�߂�l�Fconst char* �}�X�^�f�[�^��
	//--------------------------------------------------------------------------
	const char* name() const;

	//--------------------------------------------------------------------------
	//�ݒ�o�[�W�����擾 version()
	//�߂�l�Fconst char* �o�[�W�����i�f�[�^�Ȃ��A�܂��ُ͈�̏ꍇ��MasterDef::UNKNOWN�j
	//--------------------------------------------------------------------------
	const char* version() const;

	//--------------------------------------------------------------------------
	//�}�X�^�o�[�W�����擾 master_version()
	//�߂�l�Fconst char* �o�[�W�����i�f�[�^�Ȃ��A�܂��ُ͈�̏ꍇ��MasterDef::UNKNOWN�j
	//--------------------------------------------------------------------------
	const char* master_version() const;

	//--------------------------------------------------------------------------
	//�}�X�^�f�[�^�Z�b�g data_set()
	//�����F const char* data:�}�X�^�f�[�^
	//       long size:�f�[�^�T�C�Y(�w�b�_�܂�)
	//�߂�l�Fint �O�F�ُ�A�P�F����
	//--------------------------------------------------------------------------
	int data_set(const char* data, long size);

	//--------------------------------------------------------------------------
	//�}�X�^�f�[�^�擾 data_get()
	//�����F const char*& data:�}�X�^�f�[�^
	//       long& size:�f�[�^�T�C�Y(�w�b�_�܂�)
	//�߂�l�Fint �O�F�ُ�A�P�F����
	//--------------------------------------------------------------------------
	int data_get(const char*& data, long& size) const;

	//--------------------------------------------------------------------------
	//�L���J�n�N���������擾 valid_date()
	//�����F int master_id:�}�X�^�f�[�^ID �}�X�^�f�[�^����master_id�Ԗڂ̃f�[�^��
	//                     �L���J�n�N�����������擾   master_id�͂P�ȏ�
	//       Timestamp& valid_date:�L���J�n�N�A���A���A���A�����ݒ肳���
	//�߂�l�Fint �O�F�擾���s�A�P�F�擾����
	//--------------------------------------------------------------------------
	int valid_date(int master_id, Timestamp& valid_date) const;

	//--------------------------------------------------------------------------
	//�L���J�n�N�������� ����уo�[�W�����擾 valid_master()
	//�����F int master_id:�}�X�^�f�[�^ID �}�X�^�f�[�^����master_id�Ԗڂ̃f�[�^��
	//                     �L���J�n�N������������уo�[�W�������擾   master_id�͂P�ȏ�
	//       Timestamp& valid_date:�L���J�n�N�A���A���A���A�����ݒ肳���
	//       const char*& master_version:�L����挔�f�[�^�o�[�W�������ݒ肳���
	//�߂�l�Fint �O�F�擾���s�A�P�F�擾����
	//--------------------------------------------------------------------------
	int valid_master(int master_id, Timestamp& valid_date, const char*& master_version) const;

	//--------------------------------------------------------------------------
	//�L���}�X�^�擾 valid_date()
	//�����F const Timestamp& real_date:�����t �L���}�X�^���ʗp�@�N�����������w�肷��
	//       EachExtraTicketMaster& valid_master:�L���}�X�^�i�ꐢ�㕪�j
	//�߂�l�Fint �O�F�擾���s�A�P�F�擾����
	//--------------------------------------------------------------------------
	int get_master(const Timestamp& real_date, EachExtraTicketMaster& valid_master) ;

	//--------------------------------------------------------------------------
	//���탊�X�g�X�V update_kind_map()
	//�����Fbool needjudge     master_id������K�v�̎��ɁAtrue�ɂ���
	//      bool collaboration ���А���挔�����������ɁAtrue�ɂ���
	//�߂�l�Fint �O�F�X�V���s�A�P�F�X�V����
	//--------------------------------------------------------------------------
	bool update_kind_map(bool needjudge);

	//--------------------------------------------------------------------------
	//�ݒu�w�R�[�h�擾 station_setting_code()
	//�߂�l�Funsigned long �ݒu�w�R�[�h�i�f�[�^�Ȃ��A�܂��ُ͈�̏ꍇ��0�j
	//--------------------------------------------------------------------------
	unsigned long station_setting_code() const;

	//--------------------------------------------------------------------------
	//��挔�f�[�^(�z�M�f�[�^)�쐬���擾 master_data_delivery_time()
	//�߂�l�Fint
	//--------------------------------------------------------------------------
	int master_data_delivery_time(Timestamp& delivery_time) const;

	//�f�o�b�O�p
	//  void print();

private:
	ExtraTicketMaster();
	ExtraTicketMaster(const ExtraTicketMaster&);
	ExtraTicketMaster& operator=(const ExtraTicketMaster&);

	ExtraTicketMasterRep* rep_;
	static ExtraTicketMaster theInstance;

public:
	virtual const char* getMasterFileName() const;
	virtual unsigned long sum() const; 
};


}
}
}


#endif // _omronsoft_pub_extra_ticket_master_h_
// end of file
