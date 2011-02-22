//=============================================================================
//
// file :        DataBase.h
//
// description : Include for the DataBase class.
//
// project :	TANGO
//
// Author(s): andy_gotz, P.Verdier
//
// $Revision$
//
// $Log$
// Revision 2.36  2008/03/26 17:31:00  taurel
// - Committed after merge with the Release 4 branch
//
// Revision 2.35.2.8  2008/02/28 17:07:37  taurel
// - Added the StoredProcedureRelease attribute
//
// Revision 2.35.2.7  2008/01/25 12:29:50  taurel
// - Add a new command DbDeleteAllDeviceAttributeProperty
// - The DbImportDevice now also returns the device class
// - Fix bug in the DbGetHostServerList command
//
// Revision 2.35.2.6  2008/01/03 08:44:49  taurel
// - Add MySQL connection pool size definition as a command line option
// called "poolSize" with a default value of 20
//
// Revision 2.35.2.5  2007/12/19 11:48:29  taurel
// - Fix several bugs in the connection management in case of error
// - Use the same connection for the get_id() method
// - Re-code the call to the stored procedure as decribed in MySQL doc
//
// Revision 2.35.2.4  2007/12/06 16:45:03  taurel
// - Fix some bugs in the locking system
//
// Revision 2.35.2.3  2007/12/06 08:34:21  taurel
// - Add table slock to be pretected against threading within a server and againt several servers using the same DB
//
// Revision 2.35.2.2  2007/11/08 10:02:59  taurel
// Several changes to get a fully multi-threaded Database server
// - Add interceptors for thread creation and deletion
// - Use the NO_SYNC Tango serialisation model
// - Add some mutexes to protect miscellaneous pieces of code
// - Re-write the get_id() method
// - Create a MySQL connection pool
//
// Revision 2.35.2.1  2007/11/07 09:50:40  taurel
// - First changes to get a fully-threaded DB server
//
// Revision 2.35  2007/11/06 07:24:06  taurel
// - Added the DbGetDataForServerCache command (with timing stats)
// - Add timing stats for the DbPutClassProperty command
//
// Revision 2.34  2006/09/28 11:18:20  pascal_verdier
// DbGetClassForDevice and DbGetClassInheritanceForDevice commands added.
//
// Revision 2.33  2006/08/29 08:49:20  jlpons
// Moved database purge to the DB server
//
// Revision 2.32  2006/06/22 15:25:32  jlpons
// Added history commands
//
// Revision 2.31  2005/10/19 08:47:01  pascal_verdier
// Bug on put_property() for multi lines value fixed.
//
// Revision 2.30  2005/10/05 07:35:54  pascal_verdier
// Bug fixed in Timing_Info attribute read.
// ResetTimingValues command added.
// Windows compatibility added for timing attributes.
//
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//             (c) - Pascal Verdier - ESRF
//=============================================================================
#ifndef _DATABASE_H
#define _DATABASE_H

#include <tango.h>
#include <mysql.h>
#include <update_starter.h>

#ifndef WIN32
#	include <sys/time.h>
#endif

//	Add your own constants definitions here.
//-----------------------------------------------
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



namespace DataBase_ns {

/**
 * Class Description:
 * This class manage the TANGO database.
 */

/*
 *	Device States Description:
 */


class DataBase: public Tango::Device_3Impl
{
public :
	//	Add your own data members here
	//-----------------------------------------
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

	//	Here is the Start of the automatic code generation part
	//-------------------------------------------------------------	
/**
 *	@name attributes
 *	Tango::Attributs menmber data.
 */
//@{
		Tango::DevString	*attr_StoredProcedureRelease_read;
		Tango::DevDouble	*attr_Timing_average_read;
		Tango::DevDouble	*attr_Timing_minimum_read;
		Tango::DevDouble	*attr_Timing_maximum_read;
		Tango::DevDouble	*attr_Timing_calls_read;
		Tango::DevString	*attr_Timing_index_read;
		Tango::DevString	*attr_Timing_info_read;
//@}

/**
 *	@name Device properties
 *	Device properties member data.
 */
//@{
//@}

/**@name Constructors
 * Miscellaneous constructors */
//@{
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
//@}

/**@name Destructor
 * Only one desctructor is defined for this class */
//@{
/**
 * The object desctructor.
 */	
	~DataBase() {delete_device();};
/**
 *	will be called at device destruction or at init command.
 */
	void delete_device();
//@}

	
/**@name Miscellaneous methods */
//@{
/**
 *	Initialize the device
 */
	virtual void init_device();
/**
 *	Read The devive state.
 *	<Br>Command DevState 
 *
 * @return the device state read.
 */
/**
 *	Read The devive status.
 *	<Br>Command DevStatus
 *
 * @return A String containing the device state read.
 */
	virtual const char* dev_status();
/**
 *	Always executed method befor execution command method.
 */
	virtual void always_executed_hook();

//@}

/**
 * @name DataBase methods prototypes
 */

//@{
/**
 *	Hardware acquisition for attributes.
 */
	virtual void read_attr_hardware(vector<long> &attr_list);
/**
 *	Extract real attribute values for StoredProcedureRelease acquisition result.
 */
	virtual void read_StoredProcedureRelease(Tango::Attribute &attr);
/**
 *	Extract real attribute values for Timing_average acquisition result.
 */
	virtual void read_Timing_average(Tango::Attribute &attr);
/**
 *	Extract real attribute values for Timing_minimum acquisition result.
 */
	virtual void read_Timing_minimum(Tango::Attribute &attr);
/**
 *	Extract real attribute values for Timing_maximum acquisition result.
 */
	virtual void read_Timing_maximum(Tango::Attribute &attr);
/**
 *	Extract real attribute values for Timing_calls acquisition result.
 */
	virtual void read_Timing_calls(Tango::Attribute &attr);
/**
 *	Extract real attribute values for Timing_index acquisition result.
 */
	virtual void read_Timing_index(Tango::Attribute &attr);
/**
 *	Extract real attribute values for Timing_info acquisition result.
 */
	virtual void read_Timing_info(Tango::Attribute &attr);
/**
 *	Read/Write allowed for StoredProcedureRelease attribute.
 */
	virtual bool is_StoredProcedureRelease_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for Timing_average attribute.
 */
	virtual bool is_Timing_average_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for Timing_minimum attribute.
 */
	virtual bool is_Timing_minimum_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for Timing_maximum attribute.
 */
	virtual bool is_Timing_maximum_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for Timing_calls attribute.
 */
	virtual bool is_Timing_calls_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for Timing_index attribute.
 */
	virtual bool is_Timing_index_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for Timing_info attribute.
 */
	virtual bool is_Timing_info_allowed(Tango::AttReqType type);
/**
 *	Execution allowed for DbAddDevice command.
 */
	virtual bool is_DbAddDevice_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbAddServer command.
 */
	virtual bool is_DbAddServer_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteAttributeAlias command.
 */
	virtual bool is_DbDeleteAttributeAlias_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteClassAttribute command.
 */
	virtual bool is_DbDeleteClassAttribute_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteClassAttributeProperty command.
 */
	virtual bool is_DbDeleteClassAttributeProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteClassProperty command.
 */
	virtual bool is_DbDeleteClassProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteDevice command.
 */
	virtual bool is_DbDeleteDevice_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteDeviceAlias command.
 */
	virtual bool is_DbDeleteDeviceAlias_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteDeviceAttribute command.
 */
	virtual bool is_DbDeleteDeviceAttribute_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteDeviceAttributeProperty command.
 */
	virtual bool is_DbDeleteDeviceAttributeProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteDeviceProperty command.
 */
	virtual bool is_DbDeleteDeviceProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteProperty command.
 */
	virtual bool is_DbDeleteProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteServer command.
 */
	virtual bool is_DbDeleteServer_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteServerInfo command.
 */
	virtual bool is_DbDeleteServerInfo_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbExportDevice command.
 */
	virtual bool is_DbExportDevice_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbExportEvent command.
 */
	virtual bool is_DbExportEvent_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetAliasDevice command.
 */
	virtual bool is_DbGetAliasDevice_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetAttributeAlias command.
 */
	virtual bool is_DbGetAttributeAlias_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetAttributeAliasList command.
 */
	virtual bool is_DbGetAttributeAliasList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetClassAttributeList command.
 */
	virtual bool is_DbGetClassAttributeList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetClassAttributeProperty command.
 */
	virtual bool is_DbGetClassAttributeProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetClassAttributeProperty2 command.
 */
	virtual bool is_DbGetClassAttributeProperty2_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetClassAttributePropertyHist command.
 */
	virtual bool is_DbGetClassAttributePropertyHist_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetClassForDevice command.
 */
	virtual bool is_DbGetClassForDevice_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetClassInheritanceForDevice command.
 */
	virtual bool is_DbGetClassInheritanceForDevice_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetClassList command.
 */
	virtual bool is_DbGetClassList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetClassProperty command.
 */
	virtual bool is_DbGetClassProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetClassPropertyHist command.
 */
	virtual bool is_DbGetClassPropertyHist_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetClassPropertyList command.
 */
	virtual bool is_DbGetClassPropertyList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceAlias command.
 */
	virtual bool is_DbGetDeviceAlias_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceAliasList command.
 */
	virtual bool is_DbGetDeviceAliasList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceAttributeList command.
 */
	virtual bool is_DbGetDeviceAttributeList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceAttributeProperty command.
 */
	virtual bool is_DbGetDeviceAttributeProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceAttributeProperty2 command.
 */
	virtual bool is_DbGetDeviceAttributeProperty2_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceAttributePropertyHist command.
 */
	virtual bool is_DbGetDeviceAttributePropertyHist_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceClassList command.
 */
	virtual bool is_DbGetDeviceClassList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceDomainList command.
 */
	virtual bool is_DbGetDeviceDomainList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceExportedList command.
 */
	virtual bool is_DbGetDeviceExportedList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceFamilyList command.
 */
	virtual bool is_DbGetDeviceFamilyList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceInfo command.
 */
	virtual bool is_DbGetDeviceInfo_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceList command.
 */
	virtual bool is_DbGetDeviceList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceMemberList command.
 */
	virtual bool is_DbGetDeviceMemberList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceProperty command.
 */
	virtual bool is_DbGetDeviceProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDevicePropertyHist command.
 */
	virtual bool is_DbGetDevicePropertyHist_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDevicePropertyList command.
 */
	virtual bool is_DbGetDevicePropertyList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDeviceServerClassList command.
 */
	virtual bool is_DbGetDeviceServerClassList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetExportdDeviceListForClass command.
 */
	virtual bool is_DbGetExportdDeviceListForClass_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetHostList command.
 */
	virtual bool is_DbGetHostList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetHostServerList command.
 */
	virtual bool is_DbGetHostServerList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetHostServersInfo command.
 */
	virtual bool is_DbGetHostServersInfo_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetInstanceNameList command.
 */
	virtual bool is_DbGetInstanceNameList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetObjectList command.
 */
	virtual bool is_DbGetObjectList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetProperty command.
 */
	virtual bool is_DbGetProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetPropertyHist command.
 */
	virtual bool is_DbGetPropertyHist_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetPropertyList command.
 */
	virtual bool is_DbGetPropertyList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetServerInfo command.
 */
	virtual bool is_DbGetServerInfo_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetServerList command.
 */
	virtual bool is_DbGetServerList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetServerNameList command.
 */
	virtual bool is_DbGetServerNameList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbImportDevice command.
 */
	virtual bool is_DbImportDevice_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbImportEvent command.
 */
	virtual bool is_DbImportEvent_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbInfo command.
 */
	virtual bool is_DbInfo_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbPutAttributeAlias command.
 */
	virtual bool is_DbPutAttributeAlias_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbPutClassAttributeProperty command.
 */
	virtual bool is_DbPutClassAttributeProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbPutClassAttributeProperty2 command.
 */
	virtual bool is_DbPutClassAttributeProperty2_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbPutClassProperty command.
 */
	virtual bool is_DbPutClassProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbPutDeviceAlias command.
 */
	virtual bool is_DbPutDeviceAlias_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbPutDeviceAttributeProperty command.
 */
	virtual bool is_DbPutDeviceAttributeProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbPutDeviceAttributeProperty2 command.
 */
	virtual bool is_DbPutDeviceAttributeProperty2_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbPutDeviceProperty command.
 */
	virtual bool is_DbPutDeviceProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbPutProperty command.
 */
	virtual bool is_DbPutProperty_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbPutServerInfo command.
 */
	virtual bool is_DbPutServerInfo_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbUnExportDevice command.
 */
	virtual bool is_DbUnExportDevice_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbUnExportEvent command.
 */
	virtual bool is_DbUnExportEvent_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbUnExportServer command.
 */
	virtual bool is_DbUnExportServer_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for ResetTimingValues command.
 */
	virtual bool is_ResetTimingValues_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbGetDataForServerCache command.
 */
	virtual bool is_DbGetDataForServerCache_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DbDeleteAllDeviceAttributeProperty command.
 */
	virtual bool is_DbDeleteAllDeviceAttributeProperty_allowed(const CORBA::Any &any);
/**
 * This command gets the device state (stored in its <i>device_state</i> data member) and returns it to the caller.
 *	@return	State Code
 *	@exception DevFailed
 */
	virtual Tango::DevState	dev_state();
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_add_device(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_add_server(const Tango::DevVarStringArray *);
/**
 * Delete an attribute alias.
 *	@param	argin	alias name.
 *	@exception DevFailed
 */
	void	db_delete_attribute_alias(Tango::DevString);
/**
 * delete a class attribute and all its properties from
 *	the database
 *	@param	argin	device
 *	@exception DevFailed
 */
	void	db_delete_class_attribute(const Tango::DevVarStringArray *);
/**
 * delete a class attribute property from the database
 *	@param	argin	device
 *	@exception DevFailed
 */
	void	db_delete_class_attribute_property(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_delete_class_property(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	device name
 *	@exception DevFailed
 */
	void	db_delete_device(Tango::DevString);
/**
 * Delete a device alias.
 *	@param	argin	alias name
 *	@exception DevFailed
 */
	void	db_delete_device_alias(Tango::DevString);
/**
 * delete a device attribute and all its properties from
 *	the database
 *	@param	argin	device
 *	@exception DevFailed
 */
	void	db_delete_device_attribute(const Tango::DevVarStringArray *);
/**
 * delete a device attribute property from the database
 *	@param	argin	device
 *	@exception DevFailed
 */
	void	db_delete_device_attribute_property(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_delete_device_property(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_delete_property(const Tango::DevVarStringArray *);
/**
 * delete server from the database, do not delete device properties
 *	@param	argin	server name
 *	@exception DevFailed
 */
	void	db_delete_server(Tango::DevString);
/**
 * delete info related to a server
 *	@param	argin	server name
 *	@exception DevFailed
 */
	void	db_delete_server_info(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_export_device(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	event channel or factory
 *	@exception DevFailed
 */
	void	db_export_event(const Tango::DevVarStringArray *);
/**
 * Return the device name for specified alias.
 *	@param	argin	specified alias.
 *	@return	Device name found.
 *	@exception DevFailed
 */
	Tango::DevString	db_get_alias_device(Tango::DevString);
/**
 * Get the device attribute name for the given alias. If no alias corresponds then return an empty string.
 *	@param	argin	alias
 *	@return	attribute name
 *	@exception DevFailed
 */
	Tango::DevString	db_get_attribute_alias(Tango::DevString);
/**
 * 
 *	@param	argin	attribute alias
 *	@return	attribute name
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_attribute_alias_list(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_class_attribute_list(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_class_attribute_property(const Tango::DevVarStringArray *);
/**
 * This command supports array property compared to the old command called
 *	DbGetClassAttributeProperty. The old command has not been deleted from the
 *	server for compatibility reasons.
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_class_attribute_property2(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_class_attribute_property_hist(const Tango::DevVarStringArray *);
/**
 * Search the class of the specified device.
 *	@param	argin	Device name
 *	@return	Class off the specified device
 *	@exception DevFailed
 */
	Tango::DevString	db_get_class_for_device(Tango::DevString);
/**
 * Search the class inheritance of the specified device.
 *	@param	argin	Device name
 *	@return	Classes off the specified device.\n[0] - is the class of the device.\n[1] - is the class from the device class is inherited.\n........and so on
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_class_inheritance_for_device(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_class_list(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_class_property(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_class_property_hist(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_class_property_list(Tango::DevString);
/**
 * Return alias for device name if found.
 *	@param	argin	The device name
 *	@return	The alias found
 *	@exception DevFailed
 */
	Tango::DevString	db_get_device_alias(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_alias_list(Tango::DevString);
/**
 * return list of attributes for device which match the
 *	wildcard
 *	@param	argin	device name
 *	@return	attribute wildcard
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_attribute_list(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_attribute_property(const Tango::DevVarStringArray *);
/**
 * Retrieve device attribute properties. This command has the possibility to retrieve
 *	device attribute properties which are arrays. It is not possible with the old
 *	DbGetDeviceAttributeProperty command. Nevertheless, the old command has not been
 *	deleted for compatibility reason
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_attribute_property2(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_attribute_property_hist(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_class_list(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_domain_list(Tango::DevString);
/**
 * Get a list of exported devices whose names satisfy the filter (wildcard is
 *	@param	argin	filter
 *	@return	list of exported devices
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_exported_list(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_family_list(Tango::DevString);
/**
 * Returns info from DbImportDevice and started/stopped dates.
 *	@param	argin	Device name
 *	@return	Info from DbImportDevice and started/stopped dates.
 *	@exception DevFailed
 */
	Tango::DevVarLongStringArray	*db_get_device_info(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_list(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_member_list(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_property(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_property_hist(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_property_list(const Tango::DevVarStringArray *);
/**
 * return list of device classes for a device server
 *	@param	argin	device server instance name
 *	@return	list of classes for this device server
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_device_server_class_list(Tango::DevString);
/**
 * Query the database for device exported for the specified class.
 *	@param	argin	Class name
 *	@return	Device exported list
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_exportd_device_list_for_class(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_host_list(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_host_server_list(Tango::DevString);
/**
 * return info about all servers running on specified host, name, mode and level
 *	@param	argin	host name
 *	@return	server info for all servers running on specified host
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_host_servers_info(Tango::DevString);
/**
 * Returns the instance names found for specified server.
 *	@param	argin	server name
 *	@return	The instance names found for specified server.
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_instance_name_list(Tango::DevString);
/**
 * DataBase methods prototypes
 *	@param	argin	wild card
 *	@return	list of object names
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_object_list(Tango::DevString);
/**
 * DataBase methods prototypes
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_property(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_property_hist(const Tango::DevVarStringArray *);
/**
 * DataBase methods prototypes
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_property_list(const Tango::DevVarStringArray *);
/**
 * return info about host, mode and level for specified server
 *	@param	argin	server name
 *	@return	server info
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_server_info(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_server_list(Tango::DevString);
/**
 * Returns the list of server names found for the wildcard specified.
 *	It returns only the server executable name without instance name as DbGetServerList.
 *	@param	argin	wildcard for server names.
 *	@return	server names found.
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_server_name_list(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarLongStringArray	*db_import_device(Tango::DevString);
/**
 * 
 *	@param	argin	name of event channel or factory
 *	@return	export information e.g. IOR
 *	@exception DevFailed
 */
	Tango::DevVarLongStringArray	*db_import_event(Tango::DevString);
/**
 * 
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_info();
/**
 * Define an alias for an attribute
 *	@param	argin	attribute name, alias
 *	@exception DevFailed
 */
	void	db_put_attribute_alias(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_put_class_attribute_property(const Tango::DevVarStringArray *);
/**
 * This command adds support for array properties compared to the previous one
 *	called DbPutClassAttributeProperty. The old comman is still there for compatibility reason
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_put_class_attribute_property2(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_put_class_property(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	device name
 *	@exception DevFailed
 */
	void	db_put_device_alias(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_put_device_attribute_property(const Tango::DevVarStringArray *);
/**
 * Put device attribute property. This command adds the possibility to have attribute property
 *	which are arrays. Not possible with the old DbPutDeviceAttributeProperty command.
 *	This old command is not deleted for compatibility reasons.
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_put_device_attribute_property2(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_put_device_property(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_put_property(const Tango::DevVarStringArray *);
/**
 * update server info including host, mode and level
 *	@param	argin	server info
 *	@exception DevFailed
 */
	void	db_put_server_info(const Tango::DevVarStringArray *);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_un_export_device(Tango::DevString);
/**
 * 
 *	@param	argin	name of event channel or factory to unexport
 *	@exception DevFailed
 */
	void	db_un_export_event(Tango::DevString);
/**
 * 
 *	@param	argin	
 *	@exception DevFailed
 */
	void	db_un_export_server(Tango::DevString);
/**
 * Reset the timing attribute values.
 *	@exception DevFailed
 */
	void	reset_timing_values();
/**
 * This command returns all the data needed by a device server process during its
 *	startup sequence. The aim of this command is to minimize database access during
 *	device server startup sequence.
 *	@param	argin	Elt[0] = DS name (exec_name/inst_name), Elt[1] = Host name
 *	@return	All the data needed by the device server during its startup sequence. Precise list depend on the device server
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*db_get_data_for_server_cache(const Tango::DevVarStringArray *);
/**
 * Delete all attribute properties for the specified device attribute(s)
 *	@param	argin	str[0] = device name, str[1]...str[n] = attribute name(s)
 *	@exception DevFailed
 */
	void	db_delete_all_device_attribute_property(const Tango::DevVarStringArray *);

//@}

	//	Here is the end of the automatic code generation part
	//-------------------------------------------------------------	



protected :	
	//	Add your own data members here
	//-----------------------------------------
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

	typedef struct DbConnection
	{
		MYSQL 			*db;
		omni_semaphore 	the_sema;
	};
	DbConnection	*conn_pool;
	int				last_sem_wait;
	static int		conn_pool_size;
	char 			*stored_release_ptr;
	char			stored_release[128];

	omni_mutex		timing_stats_mutex;
	omni_mutex		starter_mutex;
	omni_mutex		sem_wait_mutex;
				
	void create_connection_pool(char *,char *);

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

};

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
	
	virtual void create_thread() {mysql_thread_init();}
	virtual void delete_thread() {mysql_thread_end();}
};

}	//	namespace

#endif	// _DATABASE_H
