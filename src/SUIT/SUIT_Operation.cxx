/**
*  SALOME SalomeApp
*
*  Copyright (C) 2005  CEA/DEN, EDF R&D
*
*
*
*  File   : SUIT_Operation.h
*  Author : Unknown
*  Module : SALOME
*/

#include "SUIT_Operation.h"

#include "SUIT_Study.h"
#include "SUIT_Application.h"
#include "SUIT_MessageBox.h"
#include "SUIT_Desktop.h"

/*!
 * \brief Constructor
  * \param SUIT_Application - application for this operation
*
* Constructs an empty operation. Constructor should work very fast because many
* operators may be created after starting application but only several from them
* may be used. As result this constructor stores given application in myApp field
* and set Waiting status.
*/
SUIT_Operation::SUIT_Operation( SUIT_Application* app )
: QObject(),
myApp( app ),
myStudy( 0 ),
myState( Waiting )
{
}

/*!
   * \brief Destructor
*/
SUIT_Operation::~SUIT_Operation()
{
}

/*!
 * \brief Returns operation study
  * \return Pointer to study
*
* Get study corresponding to this operation i.e. study which starts this operation.
*/
SUIT_Study* SUIT_Operation::study() const
{
  return myStudy;
}

/*!
 * \brief Sets operation study
  * \param theStudy - study corresponding to this operation
*
* Sets study corresponding to this operation i.e. study which starts this operation.
*/
void SUIT_Operation::setStudy( SUIT_Study* theStudy )
{
  myStudy = theStudy;
}

/*!
 * \brief Gets application
  * \return Pointer to application
*
* Gets application for this operation
*/
SUIT_Application* SUIT_Operation::application() const
{
  return myApp;
}

/*!
 * \brief Sets application
  * \param theApp - application for this operation
*
* Gets application for this operation
*/
void SUIT_Operation::setApplication( SUIT_Application* theApp )
{
  myApp = theApp;
}

/*!
 * \brief Gets state of operation
  * \return Value from OperationState enumeration
*
* Gets state of operation (see OperationState enumeration)
*/
SUIT_Operation::OperationState SUIT_Operation::state() const
{
  return myState;
}

/*!
 * \brief Sets state of operation
  * \param theState - state of operation to be set
*
*  Sets state of operation (see OperationState enumeration)
*/
void SUIT_Operation::setState( const SUIT_Operation::OperationState theState )
{
  myState = theState;
}

/*!
 * \brief Starts operation
*
* Public slot. Verifies whether operation can be started and starts operation.
* This slot is not virtual and cannot be redefined. Redefine startOperation method
* to change behavior of operation. There is no point in using this method. It would
* be better to inherit own operator from base one and redefine startOperation method
* instead.
*/
void SUIT_Operation::start()
{
  if ( study() )
    study()->start( this );
  else
  {
    startOperation();
    emit started( this );
  }
}

/*!
 * \brief Aborts operation
*
* Public slot. Aborts operation. This slot is not virtual and cannot be redefined.
* Redefine abortOperation method to change behavior of operation instead
*/
void SUIT_Operation::abort()
{
  if ( study() )
    study()->abort( this );
  else
  {
    abortOperation();
    myState = Waiting;
    emit aborted( this );
    emit stopped( this );
  }
}

/*!
 * \brief Commits operation
*
* Public slot. Commits operation. This slot is not virtual and cannot be redefined.
* Redefine commitOperation method to change behavior of operation instead
*/
void SUIT_Operation::commit()
{
  if ( study() )
    study()->commit( this );
  else
  {
    commitOperation();
    myState = Waiting;
    emit committed( this );
    emit stopped( this );
  }
}

/*!
 * \brief Resumes operation
*
* Public slot. Resumes operation. This slot is called when operation is resumed after
* previous suspending. This slot is not virtual and cannot be redefined. Redefine
* resumeOperation method to change behavior of operation instead
*/
void SUIT_Operation::resume()
{
  if ( study() )
    study()->resume( this );
  else
  {
    resumeOperation();
    myState = Running;
    emit resumed( this );
  }
}

/*!
 * \brief Suspend operation.
*
* Public slot. Suspend operation. This slot is called when operation is suspended
* (for starting other one, for example) This slot is not virtual and cannot be
* redefined. Redefine suspendOperation method to change behavior of operation instead
*/
void SUIT_Operation::suspend()
{
  if ( study() )
    study()->suspend( this );
  else
  {
    suspendOperation();
    myState = Suspended;
    emit suspended( this );
  }
}

/*!
 * \brief Verifies whether operator is ready to start.
 * \return TRUE if operation is ready to start
*
* Default implementation returns TRUE. Redefine this method to add own verifications
*/
bool SUIT_Operation::isReadyToStart() const
{
  return true;
}

/*!
 * \brief Virtual method called when operation is started
*
* Virtual method called when operation started (see start() method for more description)
*/
void SUIT_Operation::startOperation()
{
  emit callSlot();
  commit();
}

/*!
 * \brief Virtual method called when operation aborted
*
* Virtual method called when operation aborted (see abort() method for more description)
*/
void SUIT_Operation::abortOperation()
{
}

/*!
 * \brief Virtual method called when operation resumed
*
* Virtual method called when operation resumed (see resume() method for more description)
*/
void SUIT_Operation::resumeOperation()
{
}

/*!
 * \brief Virtual method called when operation suspended
*
* Virtual method called when operation suspended (see suspend() method for more description)
*/
void SUIT_Operation::suspendOperation()
{
}

/*!
 * \brief Virtual method called when operation committed
*
* Virtual method called when operation committed (see commit() method for more description)
*/
void SUIT_Operation::commitOperation()
{
}

/*!
 * \brief Sets slot which is called when operation is started
  * \param theReceiver - object containing slot
  * \param theSlot - slot of theReceiver object
  * \return TR if slot was connected successfully, FALSE otherwise
*
* Sets slot which is called when operation is started. There is no point in
* using this method. It would be better to inherit own operator from base
* one and redefine startOperation method
*/
bool SUIT_Operation::setSlot( const QObject* theReceiver, const char* theSlot )
{
  return connect( this, SIGNAL( callSlot() ), theReceiver, theSlot );
}

/*!
 * \brief Verifies whether given operator is valid for this one
  * \param theOtherOp - other operation
  * \return Returns TRUE if the given operator is valid for this one
*
* Verifies whether given operator is valid for this one (i.e. can be started "above"
* this operator)
*/
bool SUIT_Operation::isValid( SUIT_Operation* ) const
{
  return false;
}

/*!
 * \brief Verifies whether this operator can be always started above any already runnig one
  * \return Returns TRUE if current operation must not be checked for ActiveOperation->IsValid( this )
*
* This method must be redefined in derived operation if operation of derived class
* must be always can start above any launched one. Default implementation returns FALSE,
* so it is being checked for IsValid, but some operations may overload IsGranted()
* In this case they will always start, no matter what operation is running.
*/
bool SUIT_Operation::isGranted() const
{
  return false;
}

/*!
 * \brief Verifies whether operation is an active one (state()==Running)
  * \return TRUE if operation is active, FALSE otherwise
*
* Verifies whether operation is an active on. Returns TRUE if state of operator
* is Running
*/
bool SUIT_Operation::isActive() const
{
  return state()==Running;
}

/*!
 * \brief Starts operator above this one
  * \param theOp - operation to be started
*
* Start operator above this one. Use this method if you want to call other operator
* from this one
*/
void SUIT_Operation::start( SUIT_Operation* op )
{
  if ( !op )
    return;
    
  if ( study() )
    study()->start( op, false );
  else
  {
    connect( this, SIGNAL( stopped( SUIT_Operation* ) ), op, SLOT( abort() ) );
    op->start();
  }
}

/*!
 * \brief Sets execution status
  * \param theStatus - execution status
*
* Sets myExecStatus to the given value
*/
void SUIT_Operation::setExecStatus( const int theVal )
{
  myExecStatus = (ExecStatus)theVal;
}

/*!
 * \brief Gets execution status
 * \return Execution status
*
* Gets execution status
*/
int SUIT_Operation::execStatus() const
{
  return myExecStatus;
}








