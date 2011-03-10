/*----- PROTECTED REGION ID(DataBase.h) ENABLED START -----*/
//=============================================================================
//
// file :        DataBase.h
//
// description : Include for the DataBase class.
//
// project :     TANGO.
//
// $Author$
//
// $Revision$
// $Date$
//
// SVN only:
// $HeadURL:  $
//
// CVS only:
// $Source:  $
// $Log:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#ifndef DATABASE_H
#define DATABASE_H


#include <tango.h>
#ifdef WIN32
#include <my_global.h>
#endif
#include <mysql.h>
#include <update_starter.h>


#define DB_SQLError 				"DB_SQLError"
#define DB_IncorrectArguments		"DB_IncorrectArguments"
#define DB_IncorrectDeviceName		"DB_IncorrectDeviceName"
#define DB_IncorrectServerName		"DB_IncorrectServerName"
#define DB_DeviceNotDefined			"DB_DeviceNotDefined"
#define DB_AliasNotDefined			"DB_AliasNotDefined"
#define	DB_NoFreeMySQLConnection	"DB_NoFreeMySQLConnection"
#define	DB_MySQLLibNotThreadSafe	"DB_MySQLLibNotThreadSafe"

#define	STARTER_DEVNAME_HEADER		"tango/admin/"

#define	DEFAULT_CONN_POOL_SIZE		20

//	Define time measuremnt type (depends on OS)
#ifndef WIN32

#	define	TimeVal	struct timeval
#	define	GetTime(t)	gettimeofday(&t, NULL);
#	define	Elapsed(before, after)	\
		1000.0*(after.tv_sec-before.tv_sec) + \
		((double)after.tv_usec-before.tv_usec) / 1000

#else

static LARGE_INTEGER	cpu_freq;
#	define	TimeVal	LARGE_INTEGER
#	define	GetTime(t)	w_gettimeofday(&t);
#	define	Elapsed(before, after)		\
		(cpu_freq.QuadPart==0) ? 0.0 :	\
			(double) (after.QuadPart - before.QuadPart)/cpu_freq.QuadPart * 1000;

#endif	/*	WIN32		*/




/*----- PROTECTED REGION END -----*/


/**
 *	DataBase class Description:
 *	This class manage the TANGO database.
 */

namespace DataBase_ns
{
	/*----- PROTECTED REGION ID(DataBase::Additional Class Declarations) ENABLED START -----*/

		//		Additional Class Declarations
class DummyDev: public Tango::Connection
{
public:
	DummyDev():Tango::Connection(true) {};

	virtual string get_corba_name(bool) {string str;return str;}
	virtual string build_corba_name() {string str;return str;}
	virtual int get_lock_ctr() {return 0;}
	virtual void set_lock_ctr(int) {};

	virtual string dev_name() {string str;return str;}

	int get_env_var(const char *cc,string &str_ref) {return Tango::Connection::get_env_var(cc,str_ref);}
};

	/*----- PROTECTED REGION END -----*/	//	DataBase::Additional Class Declarations


class DataBase : public Tango::Device_4Impl
{


	/*----- PROTECTED REGION ID(DataBase::Data Members) ENABLED START -----*/

	//		Add your own data members
public:
	/**
	 *	current incarnation of database
	 */
	static string db_name;

	/**
	 *	Will be set by property of Default object
	 */
	bool	fireToStarter;
	
	/**
	 *	Database value history depth
	 */
	int	historyDepth;
	
	/**
	 *	Shared data for update starter thread
	 */
	UpdStarterData	*starter_shared;
	/**
	 *	update starter thread instance
	 */
	UpdateStarter	*upd_starter_thread;

	/*
	 * timing related variables
	 */
	typedef struct timing_stats_struct
	{
		double calls;
		double total_elapsed;
		double average;
		double minimum;
		double maximum;
	} TimingStatsStruct;

	map<std::string,TimingStatsStruct*> timing_stats_map; 

	int timing_stats_size;
	double *timing_stats_average;
	double *timing_stats_minimum;
	double *timing_stats_maximum;
	double *timing_stats_calls;
	char **timing_stats_index;


	/*----- PROTECTED REGION END -----*/	//	DataBase::Data Members


//	Device property data members
public:	

//	Attribute data members
public:
	Tango::DevString	*attr_StoredProcedureRelease_read;
	Tango::DevDouble	*attr_Timing_average_read;
	Tango::DevDouble	*attr_Timing_minimum_read;
	Tango::DevDouble	*attr_Timing_maximum_read;
	Tango::DevDouble	*attr_Timing_calls_read;
	Tango::DevString	*attr_Timing_index_read;
	Tango::DevString	*attr_Timing_info_read;



//	Constructors and destructors
public:
	/**
	 * Constructs a newly allocated Command object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	DataBase(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly allocated Command object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	DataBase(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly allocated Command object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	DataBase(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The object destructor.
	 */	
	~DataBase() {delete_device();};



//	Miscellaneous methods
public:
	/**
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/**
	 *	Initialize the device
	 */
	virtual void init_device();
	/**
	 *	Read the device properties from database
	 */
	 void get_device_property();
	/**
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();


//	Attribute methods
public:
	/**
	 *	Method      : DataBase::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	virtual void read_attr_hardware(vector<long> &attr_list);


	/**
	 *	StoredProcedureRelease attribute related methods.
	 *	Description: 
	 *
	 *	Data type:	Tango::DevString
	 *	Attr type:	Scalar 
	 */
	virtual void read_StoredProcedureRelease(Tango::Attribute &attr);
	virtual bool is_StoredProcedureRelease_allowed(Tango::AttReqType type);



	/**
	 *	Timing_average attribute related methods.
	 *	Description: 
	 *
	 *	Data type:	Tango::DevDouble
	 *	Attr type:	Spectrum  max = 64
	 */
	virtual void read_Timing_average(Tango::Attribute &attr);
	virtual bool is_Timing_average_allowed(Tango::AttReqType type);



	/**
	 *	Timing_minimum attribute related methods.
	 *	Description: 
	 *
	 *	Data type:	Tango::DevDouble
	 *	Attr type:	Spectrum  max = 64
	 */
	virtual void read_Timing_minimum(Tango::Attribute &attr);
	virtual bool is_Timing_minimum_allowed(Tango::AttReqType type);



	/**
	 *	Timing_maximum attribute related methods.
	 *	Description: 
	 *
	 *	Data type:	Tango::DevDouble
	 *	Attr type:	Spectrum  max = 64
	 */
	virtual void read_Timing_maximum(Tango::Attribute &attr);
	virtual bool is_Timing_maximum_allowed(Tango::AttReqType type);



	/**
	 *	Timing_calls attribute related methods.
	 *	Description: 
	 *
	 *	Data type:	Tango::DevDouble
	 *	Attr type:	Spectrum  max = 64
	 */
	virtual void read_Timing_calls(Tango::Attribute &attr);
	virtual bool is_Timing_calls_allowed(Tango::AttReqType type);



	/**
	 *	Timing_index attribute related methods.
	 *	Description: 
	 *
	 *	Data type:	Tango::DevString
	 *	Attr type:	Spectrum  max = 64
	 */
	virtual void read_Timing_index(Tango::Attribute &attr);
	virtual bool is_Timing_index_allowed(Tango::AttReqType type);



	/**
	 *	Timing_info attribute related methods.
	 *	Description: 
	 *
	 *	Data type:	Tango::DevString
	 *	Attr type:	Spectrum  max = 64
	 */
	virtual void read_Timing_info(Tango::Attribute &attr);
	virtual bool is_Timing_info_allowed(Tango::AttReqType type);



	/**
	 *	Method      : DataBase::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
		void add_dynamic_attributes();

//	Command related methods
public: 
	/**
	 *	Command State related methods.
	 */
	Tango::DevState dev_state();


	/**
	 *	Command DbAddDevice related methods.
	 */
	void db_add_device(const Tango::DevVarStringArray *argin);
	virtual bool is_DbAddDevice_allowed(const CORBA::Any &any);

	/**
	 *	Command DbAddServer related methods.
	 */
	void db_add_server(const Tango::DevVarStringArray *argin);
	virtual bool is_DbAddServer_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteAttributeAlias related methods.
	 */
	void db_delete_attribute_alias(Tango::DevString argin);
	virtual bool is_DbDeleteAttributeAlias_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteClassAttribute related methods.
	 */
	void db_delete_class_attribute(const Tango::DevVarStringArray *argin);
	virtual bool is_DbDeleteClassAttribute_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteClassAttributeProperty related methods.
	 */
	void db_delete_class_attribute_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbDeleteClassAttributeProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteClassProperty related methods.
	 */
	void db_delete_class_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbDeleteClassProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteDevice related methods.
	 */
	void db_delete_device(Tango::DevString argin);
	virtual bool is_DbDeleteDevice_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteDeviceAlias related methods.
	 */
	void db_delete_device_alias(Tango::DevString argin);
	virtual bool is_DbDeleteDeviceAlias_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteDeviceAttribute related methods.
	 */
	void db_delete_device_attribute(const Tango::DevVarStringArray *argin);
	virtual bool is_DbDeleteDeviceAttribute_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteDeviceAttributeProperty related methods.
	 */
	void db_delete_device_attribute_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbDeleteDeviceAttributeProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteDeviceProperty related methods.
	 */
	void db_delete_device_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbDeleteDeviceProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteProperty related methods.
	 */
	void db_delete_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbDeleteProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteServer related methods.
	 */
	void db_delete_server(Tango::DevString argin);
	virtual bool is_DbDeleteServer_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteServerInfo related methods.
	 */
	void db_delete_server_info(Tango::DevString argin);
	virtual bool is_DbDeleteServerInfo_allowed(const CORBA::Any &any);

	/**
	 *	Command DbExportDevice related methods.
	 */
	void db_export_device(const Tango::DevVarStringArray *argin);
	virtual bool is_DbExportDevice_allowed(const CORBA::Any &any);

	/**
	 *	Command DbExportEvent related methods.
	 */
	void db_export_event(const Tango::DevVarStringArray *argin);
	virtual bool is_DbExportEvent_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetAliasDevice related methods.
	 */
	Tango::DevString db_get_alias_device(Tango::DevString argin);
	virtual bool is_DbGetAliasDevice_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetAttributeAlias related methods.
	 */
	Tango::DevString db_get_attribute_alias(Tango::DevString argin);
	virtual bool is_DbGetAttributeAlias_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetAttributeAliasList related methods.
	 */
	Tango::DevVarStringArray *db_get_attribute_alias_list(Tango::DevString argin);
	virtual bool is_DbGetAttributeAliasList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetClassAttributeList related methods.
	 */
	Tango::DevVarStringArray *db_get_class_attribute_list(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetClassAttributeList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetClassAttributeProperty related methods.
	 */
	Tango::DevVarStringArray *db_get_class_attribute_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetClassAttributeProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetClassAttributeProperty2 related methods.
	 */
	Tango::DevVarStringArray *db_get_class_attribute_property2(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetClassAttributeProperty2_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetClassAttributePropertyHist related methods.
	 */
	Tango::DevVarStringArray *db_get_class_attribute_property_hist(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetClassAttributePropertyHist_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetClassForDevice related methods.
	 */
	Tango::DevString db_get_class_for_device(Tango::DevString argin);
	virtual bool is_DbGetClassForDevice_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetClassInheritanceForDevice related methods.
	 */
	Tango::DevVarStringArray *db_get_class_inheritance_for_device(Tango::DevString argin);
	virtual bool is_DbGetClassInheritanceForDevice_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetClassList related methods.
	 */
	Tango::DevVarStringArray *db_get_class_list(Tango::DevString argin);
	virtual bool is_DbGetClassList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetClassProperty related methods.
	 */
	Tango::DevVarStringArray *db_get_class_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetClassProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetClassPropertyHist related methods.
	 */
	Tango::DevVarStringArray *db_get_class_property_hist(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetClassPropertyHist_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetClassPropertyList related methods.
	 */
	Tango::DevVarStringArray *db_get_class_property_list(Tango::DevString argin);
	virtual bool is_DbGetClassPropertyList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceAlias related methods.
	 */
	Tango::DevString db_get_device_alias(Tango::DevString argin);
	virtual bool is_DbGetDeviceAlias_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceAliasList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_alias_list(Tango::DevString argin);
	virtual bool is_DbGetDeviceAliasList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceAttributeList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_attribute_list(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetDeviceAttributeList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceAttributeProperty related methods.
	 */
	Tango::DevVarStringArray *db_get_device_attribute_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetDeviceAttributeProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceAttributeProperty2 related methods.
	 */
	Tango::DevVarStringArray *db_get_device_attribute_property2(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetDeviceAttributeProperty2_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceAttributePropertyHist related methods.
	 */
	Tango::DevVarStringArray *db_get_device_attribute_property_hist(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetDeviceAttributePropertyHist_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceClassList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_class_list(Tango::DevString argin);
	virtual bool is_DbGetDeviceClassList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceDomainList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_domain_list(Tango::DevString argin);
	virtual bool is_DbGetDeviceDomainList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceExportedList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_exported_list(Tango::DevString argin);
	virtual bool is_DbGetDeviceExportedList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceFamilyList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_family_list(Tango::DevString argin);
	virtual bool is_DbGetDeviceFamilyList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceInfo related methods.
	 */
	Tango::DevVarLongStringArray *db_get_device_info(Tango::DevString argin);
	virtual bool is_DbGetDeviceInfo_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_list(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetDeviceList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceWideList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_wide_list(Tango::DevString argin);
	virtual bool is_DbGetDeviceWideList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceMemberList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_member_list(Tango::DevString argin);
	virtual bool is_DbGetDeviceMemberList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceProperty related methods.
	 */
	Tango::DevVarStringArray *db_get_device_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetDeviceProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDevicePropertyHist related methods.
	 */
	Tango::DevVarStringArray *db_get_device_property_hist(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetDevicePropertyHist_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDevicePropertyList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_property_list(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetDevicePropertyList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDeviceServerClassList related methods.
	 */
	Tango::DevVarStringArray *db_get_device_server_class_list(Tango::DevString argin);
	virtual bool is_DbGetDeviceServerClassList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetExportdDeviceListForClass related methods.
	 */
	Tango::DevVarStringArray *db_get_exportd_device_list_for_class(Tango::DevString argin);
	virtual bool is_DbGetExportdDeviceListForClass_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetHostList related methods.
	 */
	Tango::DevVarStringArray *db_get_host_list(Tango::DevString argin);
	virtual bool is_DbGetHostList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetHostServerList related methods.
	 */
	Tango::DevVarStringArray *db_get_host_server_list(Tango::DevString argin);
	virtual bool is_DbGetHostServerList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetHostServersInfo related methods.
	 */
	Tango::DevVarStringArray *db_get_host_servers_info(Tango::DevString argin);
	virtual bool is_DbGetHostServersInfo_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetInstanceNameList related methods.
	 */
	Tango::DevVarStringArray *db_get_instance_name_list(Tango::DevString argin);
	virtual bool is_DbGetInstanceNameList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetObjectList related methods.
	 */
	Tango::DevVarStringArray *db_get_object_list(Tango::DevString argin);
	virtual bool is_DbGetObjectList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetProperty related methods.
	 */
	Tango::DevVarStringArray *db_get_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetPropertyHist related methods.
	 */
	Tango::DevVarStringArray *db_get_property_hist(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetPropertyHist_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetPropertyList related methods.
	 */
	Tango::DevVarStringArray *db_get_property_list(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetPropertyList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetServerInfo related methods.
	 */
	Tango::DevVarStringArray *db_get_server_info(Tango::DevString argin);
	virtual bool is_DbGetServerInfo_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetServerList related methods.
	 */
	Tango::DevVarStringArray *db_get_server_list(Tango::DevString argin);
	virtual bool is_DbGetServerList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetServerNameList related methods.
	 */
	Tango::DevVarStringArray *db_get_server_name_list(Tango::DevString argin);
	virtual bool is_DbGetServerNameList_allowed(const CORBA::Any &any);

	/**
	 *	Command DbImportDevice related methods.
	 */
	Tango::DevVarLongStringArray *db_import_device(Tango::DevString argin);
	virtual bool is_DbImportDevice_allowed(const CORBA::Any &any);

	/**
	 *	Command DbImportEvent related methods.
	 */
	Tango::DevVarLongStringArray *db_import_event(Tango::DevString argin);
	virtual bool is_DbImportEvent_allowed(const CORBA::Any &any);

	/**
	 *	Command DbInfo related methods.
	 */
	Tango::DevVarStringArray *db_info();
	virtual bool is_DbInfo_allowed(const CORBA::Any &any);

	/**
	 *	Command DbPutAttributeAlias related methods.
	 */
	void db_put_attribute_alias(const Tango::DevVarStringArray *argin);
	virtual bool is_DbPutAttributeAlias_allowed(const CORBA::Any &any);

	/**
	 *	Command DbPutClassAttributeProperty related methods.
	 */
	void db_put_class_attribute_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbPutClassAttributeProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbPutClassAttributeProperty2 related methods.
	 */
	void db_put_class_attribute_property2(const Tango::DevVarStringArray *argin);
	virtual bool is_DbPutClassAttributeProperty2_allowed(const CORBA::Any &any);

	/**
	 *	Command DbPutClassProperty related methods.
	 */
	void db_put_class_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbPutClassProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbPutDeviceAlias related methods.
	 */
	void db_put_device_alias(const Tango::DevVarStringArray *argin);
	virtual bool is_DbPutDeviceAlias_allowed(const CORBA::Any &any);

	/**
	 *	Command DbPutDeviceAttributeProperty related methods.
	 */
	void db_put_device_attribute_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbPutDeviceAttributeProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbPutDeviceAttributeProperty2 related methods.
	 */
	void db_put_device_attribute_property2(const Tango::DevVarStringArray *argin);
	virtual bool is_DbPutDeviceAttributeProperty2_allowed(const CORBA::Any &any);

	/**
	 *	Command DbPutDeviceProperty related methods.
	 */
	void db_put_device_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbPutDeviceProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbPutProperty related methods.
	 */
	void db_put_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbPutProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbPutServerInfo related methods.
	 */
	void db_put_server_info(const Tango::DevVarStringArray *argin);
	virtual bool is_DbPutServerInfo_allowed(const CORBA::Any &any);

	/**
	 *	Command DbUnExportDevice related methods.
	 */
	void db_un_export_device(Tango::DevString argin);
	virtual bool is_DbUnExportDevice_allowed(const CORBA::Any &any);

	/**
	 *	Command DbUnExportEvent related methods.
	 */
	void db_un_export_event(Tango::DevString argin);
	virtual bool is_DbUnExportEvent_allowed(const CORBA::Any &any);

	/**
	 *	Command DbUnExportServer related methods.
	 */
	void db_un_export_server(Tango::DevString argin);
	virtual bool is_DbUnExportServer_allowed(const CORBA::Any &any);

	/**
	 *	Command ResetTimingValues related methods.
	 */
	void reset_timing_values();
	virtual bool is_ResetTimingValues_allowed(const CORBA::Any &any);

	/**
	 *	Command DbGetDataForServerCache related methods.
	 */
	Tango::DevVarStringArray *db_get_data_for_server_cache(const Tango::DevVarStringArray *argin);
	virtual bool is_DbGetDataForServerCache_allowed(const CORBA::Any &any);

	/**
	 *	Command DbDeleteAllDeviceAttributeProperty related methods.
	 */
	void db_delete_all_device_attribute_property(const Tango::DevVarStringArray *argin);
	virtual bool is_DbDeleteAllDeviceAttributeProperty_allowed(const CORBA::Any &any);

	/**
	 *	Command DbMySqlSelect related methods.
	 */
	Tango::DevVarLongStringArray *db_my_sql_select(Tango::DevString argin);
	virtual bool is_DbMySqlSelect_allowed(const CORBA::Any &any);



	/*----- PROTECTED REGION ID(DataBase::Additional Method prototypes) ENABLED START -----*/

	//	Additional Method prototypes
protected :	
	unsigned long	mysql_svr_version;
	
	bool check_device_name(string &);
	bool device_name_to_dfm(string &device_name, char domain[], char family[], char member[]);
	string replace_wildcard(const char*);
	Tango::DevString db_get_device_host(Tango::DevString,int con_nb=-1);
	string escape_string(const char *string_c_str);
	void init_timing_stats();
	unsigned int get_id(const char *name,int con_nb=-1);
	void check_history_tables();
	void purge_property(const char *table,const char *field,const char *object,const char *name,int con_nb=-1);
	void purge_att_property(const char *table,const char *field,const char *object,const char *attribute,const char *name,int con_nb=-1);

	typedef struct
	{
		MYSQL 			*db;
		omni_semaphore 	the_sema;
	}DbConnection;
	DbConnection	*conn_pool;
	int				last_sem_wait;
	static int		conn_pool_size;
	char 			*stored_release_ptr;
	char			stored_release[128];

	omni_mutex		timing_stats_mutex;
	omni_mutex		starter_mutex;
	omni_mutex		sem_wait_mutex;
				
	void create_connection_pool(const char *,const char *);
	void base_connect(int);

	inline void update_timing_stats(TimeVal before, TimeVal after, std::string command)
	{
		double time_elapsed = Elapsed(before, after);
		timing_stats_mutex.lock();
		TimingStatsStruct *timing_stats = timing_stats_map[command];
		if (timing_stats != NULL)
		{
			timing_stats->calls++;
			timing_stats->total_elapsed = timing_stats->total_elapsed+time_elapsed;
			timing_stats->average = timing_stats->total_elapsed/timing_stats->calls;
			if (time_elapsed > timing_stats->maximum) timing_stats->maximum = time_elapsed;
			if (time_elapsed < timing_stats->minimum || timing_stats->minimum == 0.0) timing_stats->minimum = time_elapsed;
		}
		timing_stats_mutex.unlock();
	}
	
#ifdef WIN32
	inline static void w_gettimeofday(LARGE_INTEGER *t)
	{
		static int				status = 0;
		if (status==0)
			//	Initialize
			status = QueryPerformanceFrequency(&cpu_freq);

		if (status!=0)
			QueryPerformanceCounter(t);		//	Get micro-second time
		else
			t->QuadPart = 0;
	}
#endif

public:
	
	void simple_query(string sql_query,const char *method,int con_nb=-1);
	MYSQL_RES *query(string sql_query,const char *method,int con_nb=-1);
	static void set_conn_pool_size(int si) {conn_pool_size = si;}
	
	int get_connection();
	void release_connection(int con_nb) {conn_pool[con_nb].the_sema.post();}

	/*----- PROTECTED REGION END -----*/	//	DataBase::Additional Method prototypes

};

	/*----- PROTECTED REGION ID(DataBase::Additional Classes Definitions) ENABLED START -----*/

	//	Additional Classes definitions
class AutoLock
{
public:
	AutoLock(const char *,DataBase *);
	~AutoLock();
	
	int get_con_nb() {return con_nb;}
	
private:
	DataBase	*the_db;
	int 		con_nb;
};

class DbInter: public Tango::Interceptors
{
public:
	DbInter() {}
	~DbInter() {}
	
	virtual void create_thread() 
	{
		if (Tango::Util::instance()->is_svr_starting() == false)
			mysql_thread_init();
	}
	virtual void delete_thread()
	{
		if (Tango::Util::instance()->is_svr_starting() == false)
			mysql_thread_end();
	}
};

	/*----- PROTECTED REGION END -----*/	//	DataBase::Additional Classes Definitions

} //	namespace

#endif	//	DATABASE_H
