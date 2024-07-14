// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "ADSKEmployee.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEmployeeArxApp : public AcRxArxApp {

public:
	CEmployeeArxApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here
		try
		{
			// You *must* call On_kInitAppMsg here
			AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

			// TODO: Add your initialization code here
			AcRxObject* pSvc;

			if (!(pSvc = acrxServiceDictionary->at(ADSKEMPLOYEE_DBXSERVICE)))
			{
				// Try to load the module, if it is not yet present 
				if (!acrxDynamicLinker->loadModule(_T("ADSKEmployee.dbx"), 0))
				{

					acutPrintf(_T("Unable to load ADSKEmployee.dbx. Unloading this application...\n"));
					return (AcRx::kRetError);

				}

			}
			return (retCode);
		}
		catch (...)
		{
		}
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		try
		{
			AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

			// TODO: Unload dependencies here

			return (retCode);
		}
		catch (...)
		{
		}
	}

	virtual void RegisterServerComponents () {
	}
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CEmployeeArxApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CEmployeeArxApp, ADSKMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void EmployeeArxCREATEEMPLOYEE () {
		// Put your command code here
		Adesk::Int32 iD, cube;
		TCHAR firstName[132];
		TCHAR lastName[132];
		AcGePoint3d pt;
		try
		{
			if (acedGetInt(L"Enter Employee ID:", &iD) != RTNORM || acedGetInt(L"Enter Employee cube number:", &cube) != RTNORM || acedGetString(0, L"Enter Employee first name:", firstName) != RTNORM || acedGetString(0, L"Enter Employee last name:", lastName) != RTNORM || acedGetPoint(NULL, L"Employee position:", asDblArray(pt)) != RTNORM) {
				return;
			}

			AcDbBlockTable* pBlockTable;
			if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead) == Acad::eOk) {
				AcDbBlockTableRecord* pSpaceRecord;
				if (pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite) == Acad::eOk) {
					ADSKEmployee* pEmployee = new ADSKEmployee();

					pEmployee->setID(iD);
					pEmployee->setCube(cube);
					pEmployee->setFirstName(firstName);
					pEmployee->setLastName(lastName);
					pEmployee->setCenter(pt);

					AcDbObjectId idO;
					if (pSpaceRecord->appendAcDbEntity(idO, pEmployee) == Acad::eOk) {
						pEmployee->close();
					}
					else
					{
						delete pEmployee;
					}
					pSpaceRecord->close();
				}
				pBlockTable->close();
			}
		}
		catch (...)
		{
		}
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEmployeeArxApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEmployeeArxApp, EmployeeArx, CREATEEMPLOYEE, CREATEEMPLOYEE, ACRX_CMD_MODAL, NULL)