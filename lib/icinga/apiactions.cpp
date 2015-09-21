/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2015 Icinga Development Team (http://www.icinga.org)    *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "icinga/apiactions.hpp"
#include "icinga/service.hpp"
#include "icinga/servicegroup.hpp"
#include "icinga/hostgroup.hpp"
#include "icinga/pluginutility.hpp"
#include "icinga/checkcommand.hpp"
#include "icinga/eventcommand.hpp"
#include "icinga/notificationcommand.hpp"
#include "remote/apiaction.hpp"
#include "remote/httputility.hpp"
#include "base/utility.hpp"
#include "base/convert.hpp"
#include <fstream>

using namespace icinga;

REGISTER_APIACTION(process_check_result, "Service;Host", &ApiActions::ProcessCheckResult);
REGISTER_APIACTION(reschedule_check, "Service;Host", &ApiActions::RescheduleCheck);
REGISTER_APIACTION(delay_notifications, "Service;Host", &ApiActions::DelayNotifications);

REGISTER_APIACTION(acknowledge_problem, "Service;Host", &ApiActions::AcknowledgeProblem);
REGISTER_APIACTION(remove_acknowledgement, "Service;Host", &ApiActions::RemoveAcknowledgement);
REGISTER_APIACTION(add_comment, "Service;Host", &ApiActions::AddComment);
REGISTER_APIACTION(remove_comment, "Service;Host", &ApiActions::RemoveComment);
REGISTER_APIACTION(remove_comment_by_id, "", &ApiActions::RemoveCommentByID);
REGISTER_APIACTION(schedule_downtime, "Service;Host", &ApiActions::ScheduleDowntime);
REGISTER_APIACTION(remove_downtime, "Service;Host", &ApiActions::RemoveDowntime);
REGISTER_APIACTION(remove_downtime_by_id, "", &ApiActions::RemoveDowntimeByID);

REGISTER_APIACTION(passive_checks, "Service;Host", &ApiActions::PassiveChecks);
REGISTER_APIACTION(active_checks, "Host", &ApiActions::ActiveChecks);
REGISTER_APIACTION(notifications, "Service;Host", &ApiActions::Notifications);
REGISTER_APIACTION(flap_detection, "Service;Host", &ApiActions::FlapDetection);

//TODO-MA. Figure out how to handle modified attributes as actions
/*
REGISTER_APIACTION(change_event_handler, "Service;Host", &ApiActions::ChangeEventHandler);
REGISTER_APIACTION(change_check_command, "Service;Host", &ApiActions::ChangeCheckCommand);
REGISTER_APIACTION(change_max_check_attempts, "Service;Host", &ApiActions::ChangeMaxCheckAttempts);
REGISTER_APIACTION(change_check_period, "Service;Host", &ApiActions::ChangeCheckPeriod);
REGISTER_APIACTION(change_check_interval, "Service;Host", &ApiActions::ChangeCheckInterval);
REGISTER_APIACTION(change_retry_interval, "Service;Host", &ApiActions::ChangeRetryInterval);
*/

REGISTER_APIACTION(global_notifications, "", &ApiActions::GlobalNotifications);
REGISTER_APIACTION(global_flap_detection, "", &ApiActions::GlobalFlapDetection);
REGISTER_APIACTION(global_event_handlers, "", &ApiActions::GlobalEventHandlers);
REGISTER_APIACTION(global_performance_data, "", &ApiActions::GlobalPerformanceData);
REGISTER_APIACTION(global_executing_svc_checks, "", &ApiActions::GlobalSvcCheckExecution);
REGISTER_APIACTION(global_executing_host_checks, "", &ApiActions::GlobalHostCheckExecution);

REGISTER_APIACTION(shutdown_process, "", &ApiActions::ShutdownProcess);
REGISTER_APIACTION(restart_process, "", &ApiActions::RestartProcess);

Dictionary::Ptr ApiActions::CreateResult(int code, const String& status, const Dictionary::Ptr& additional)
{
	Dictionary::Ptr result = new Dictionary();
	result->Set("code", code);
	result->Set("status", status);

	if (additional)
		additional->CopyTo(result);

	return result;
}

Dictionary::Ptr ApiActions::RescheduleCheck(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot reschedule check for non-existent object.");

	if (Convert::ToBool(HttpUtility::GetLastParameter(params, "force")))
		checkable->SetForceNextCheck(true);

	double nextCheck;
	if (params->Contains("next_check"))
		nextCheck = HttpUtility::GetLastParameter(params, "next_check");
	else
		nextCheck = Utility::GetTime();

	checkable->SetNextCheck(nextCheck);

	return ApiActions::CreateResult(200, "Successfully rescheduled check for " + checkable->GetName() + ".");
}

Dictionary::Ptr ApiActions::ProcessCheckResult(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot process passive check result for non-existent object.");

	if (!checkable->GetEnablePassiveChecks())
		return ApiActions::CreateResult(403, "Passive checks are disabled for " + checkable->GetName());

	Host::Ptr host;
	Service::Ptr service;
	tie(host, service) = GetHostService(checkable);

	if (!params->Contains("exit_status"))
		return ApiActions::CreateResult(403, "Parameter 'exit_status' is required.");

	int exitStatus = HttpUtility::GetLastParameter(params, "exit_status");

	ServiceState state;

	if (!service) {
		if (exitStatus == 0)
			state = ServiceOK;
		else if (exitStatus == 1)
			state = ServiceCritical;
		else
			return ApiActions::CreateResult(403, "Invalid 'exit_status' for Host " + checkable->GetName() + ".");
	} else {
		state = PluginUtility::ExitStatusToState(exitStatus);
	}

	if (!params->Contains("plugin_output"))
		return ApiActions::CreateResult(403, "Parameter 'plugin_output' is required");

	CheckResult::Ptr cr = new CheckResult();
	cr->SetOutput(HttpUtility::GetLastParameter(params, "plugin_output"));
	cr->SetState(state);

	cr->SetCheckSource(HttpUtility::GetLastParameter(params, "check_source"));
	cr->SetPerformanceData(params->Get("performance_data"));
	cr->SetCommand(params->Get("check_command"));
	cr->SetExecutionEnd(HttpUtility::GetLastParameter(params, "execution_end"));
	cr->SetExecutionStart(HttpUtility::GetLastParameter(params, "execution_start"));
	cr->SetScheduleEnd(HttpUtility::GetLastParameter(params, "schedule_end"));
	cr->SetScheduleStart(HttpUtility::GetLastParameter(params, "schedule_start"));

	checkable->ProcessCheckResult(cr);

	/* Reschedule the next check. The side effect of this is that for as long
	 * as we receive passive results for a service we won't execute any
	 * active checks. */
	checkable->SetNextCheck(Utility::GetTime() + checkable->GetCheckInterval());

	return ApiActions::CreateResult(200, "Successfully processed check result for object " + checkable->GetName() + ".");
}

Dictionary::Ptr ApiActions::PassiveChecks(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("active"))
		return ApiActions::CreateResult(403, "Parameter 'active' is required");

	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot access passive checks for non-existent object.");

	checkable->SetEnablePassiveChecks(params->Get("active"));

	return ApiActions::CreateResult(200, "Successfully " + String((params->Get("active") ? "enabled" : "disabled")) + " passive checks for object " + checkable->GetName() + ".");
}

Dictionary::Ptr ApiActions::ActiveChecks(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("active"))
		return ApiActions::CreateResult(403, "Parameter 'active' is required");

	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot access passive checks for non-existent object.");

	checkable->SetEnableActiveChecks(params->Get("active"));

	return ApiActions::CreateResult(200, "Successfully " + String((params->Get("active") ? "enabled" : "disabled")) + " passive checks for object " + checkable->GetName() + ".");
}

Dictionary::Ptr ApiActions::AcknowledgeProblem(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot acknowledge problem for non-existent object.");

	if (!params->Contains("author") || !params->Contains("comment"))
		return ApiActions::CreateResult(403, "Acknowledgements require author and comment.");

	AcknowledgementType sticky = AcknowledgementNormal;
	bool notify = false;
	double timestamp = 0.0;
	if (params->Contains("sticky"))
		sticky = AcknowledgementSticky;
	if (params->Contains("notify"))
		notify = true;
	if (params->Contains("timestamp"))
		timestamp = HttpUtility::GetLastParameter(params, "timestamp");

	Host::Ptr host;
	Service::Ptr service;
	tie(host, service) = GetHostService(checkable);

	if (!service) {
		if (host->GetState() == HostUp)
			return ApiActions::CreateResult(409, "Host " + checkable->GetName() + " is UP.");
	} else {
		if (service->GetState() == ServiceOK)
			return ApiActions::CreateResult(409, "Service " + checkable->GetName() + " is OK.");
	}

	checkable->AddComment(CommentAcknowledgement, HttpUtility::GetLastParameter(params, "author"),
	    HttpUtility::GetLastParameter(params, "comment"), timestamp);
	checkable->AcknowledgeProblem(HttpUtility::GetLastParameter(params, "author"),
	    HttpUtility::GetLastParameter(params, "comment"), sticky, notify, timestamp);

	return ApiActions::CreateResult(200, "Successfully acknowledged problem for " +  checkable->GetName());
}

Dictionary::Ptr ApiActions::RemoveAcknowledgement(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot remove acknowlegement for non-existent checkable object " + object->GetName() + ".");

	checkable->ClearAcknowledgement();
	checkable->RemoveCommentsByType(CommentAcknowledgement);

	return ApiActions::CreateResult(200, "Successfully removed acknowledgement for " + checkable->GetName() + ".");
}

Dictionary::Ptr ApiActions::AddComment(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot add comment for non-existent object");

	if (!params->Contains("author") || !params->Contains("comment"))
		return ApiActions::CreateResult(403, "Comments require author and comment.");

	String comment_id = checkable->AddComment(CommentUser, HttpUtility::GetLastParameter(params, "author"),
	    HttpUtility::GetLastParameter(params, "comment"), 0);

	Comment::Ptr comment = Checkable::GetCommentByID(comment_id);
	int legacy_id = comment->GetLegacyId();

	Dictionary::Ptr additional = new Dictionary();
	additional->Set("comment_id", comment_id);
	additional->Set("legacy_id", legacy_id);

	return ApiActions::CreateResult(200, "Successfully added comment with id " +
	    Convert::ToString(legacy_id) + " for object " + checkable->GetName() + ".", additional);
}

Dictionary::Ptr ApiActions::RemoveComment(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot remove comment form non-existent object");

	checkable->RemoveAllComments();

	return ApiActions::CreateResult(200, "Successfully removed comments for " + checkable->GetName());
}

Dictionary::Ptr ApiActions::RemoveCommentByID(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("comment_id"))
		return ApiActions::CreateResult(403, "Parameter 'comment_id' is required.");

	int comment_id = HttpUtility::GetLastParameter(params, "comment_id");

	String rid = Service::GetCommentIDFromLegacyID(comment_id);

	if (rid.IsEmpty())
		return ApiActions::CreateResult(404, "Comment '" + Convert::ToString(comment_id) + "' does not exist.");

	Service::RemoveComment(rid);

	return ApiActions::CreateResult(200, "Successfully removed comment " + Convert::ToString(comment_id) + ".");
}

Dictionary::Ptr ApiActions::Notifications(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("active"))
		return ApiActions::CreateResult(403, "Parameter 'active' is required");

	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot access notifications for non-existent object");

	checkable->SetEnableNotifications(params->Get("active"));

	return ApiActions::CreateResult(200, "Successfully " + String((params->Get("active") ? "enabled" : "disabled")) + " notifications for " + checkable->GetName());
}

Dictionary::Ptr ApiActions::DelayNotifications(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot delay notifications for non-existent object");

	if (!params->Contains("timestamp"))
		return ApiActions::CreateResult(403, "A timestamp is required to delay notifications");

	BOOST_FOREACH(const Notification::Ptr& notification, checkable->GetNotifications()) {
		notification->SetNextNotification(HttpUtility::GetLastParameter(params, "timestamp"));
	}

	return ApiActions::CreateResult(200, "Successfully delayed notifications for " + checkable->GetName());
}

Dictionary::Ptr ApiActions::ScheduleDowntime(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Can't schedule downtime for non-existent object");

	if (!params->Contains("start_time") || !params->Contains("end_time") || !params->Contains("duration") ||
		!params->Contains("author") || !params->Contains("comment"))
		return ApiActions::CreateResult(404, "Options 'start_time', 'end_time', 'duration', 'author' and 'comment' are required");

	bool fixed = false;
	if (params->Contains("fixed"))
		fixed = HttpUtility::GetLastParameter(params, "fixed");

	int triggeredByLegacy = params->Contains("trigger_id") ? (int) HttpUtility::GetLastParameter(params, "trigger_id") : 0;
	String triggeredBy;
	if (triggeredByLegacy)
		triggeredBy = Service::GetDowntimeIDFromLegacyID(triggeredByLegacy);

	String downtime_id = checkable->AddDowntime(HttpUtility::GetLastParameter(params, "author"), HttpUtility::GetLastParameter(params, "comment"),
	    HttpUtility::GetLastParameter(params, "start_time"), HttpUtility::GetLastParameter(params, "end_time"),
	    fixed, triggeredBy, HttpUtility::GetLastParameter(params, "duration"));

	Downtime::Ptr downtime = Checkable::GetDowntimeByID(downtime_id);
	int legacy_id = downtime->GetLegacyId();

	Dictionary::Ptr additional = new Dictionary();
	additional->Set("downtime_id", downtime_id);
	additional->Set("legacy_id", legacy_id);

	return ApiActions::CreateResult(200, "Successfully scheduled downtime with id " +
	     Convert::ToString(legacy_id) + " for object " + checkable->GetName() + ".", additional);
}

Dictionary::Ptr ApiActions::FlapDetection(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("active"))
		return ApiActions::CreateResult(403, "Parameter 'active' is required");

	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Can't access flap detection for non-existent object");

	checkable->SetEnableFlapping(params->Get("active"));

	return ApiActions::CreateResult(200, "Successfully " + String((params->Get("active") ? "enabled" : "disabled")) + " enabled flap detection for " + checkable->GetName());
}

Dictionary::Ptr ApiActions::RemoveDowntime(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot remove downtime for non-existent object");

	checkable->RemoveAllDowntimes();

	return ApiActions::CreateResult(200, "Successfully removed downtimes for " + checkable->GetName());
}

Dictionary::Ptr ApiActions::RemoveDowntimeByID(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("downtime_id"))
		return ApiActions::CreateResult(403, "Parameter 'downtime_id' is required.");

	int downtime_id = HttpUtility::GetLastParameter(params, "downtime_id");

	String rid = Service::GetDowntimeIDFromLegacyID(downtime_id);
	if (rid.IsEmpty())
		return ApiActions::CreateResult(404, "Downtime '" + Convert::ToString(downtime_id) + "' does not exist.");

	Service::RemoveDowntime(rid, true);

	return ApiActions::CreateResult(200, "Successfully removed downtime " + Convert::ToString(downtime_id) + ".");
}

Dictionary::Ptr ApiActions::GlobalNotifications(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("active"))
		return ApiActions::CreateResult(403, "Parameter 'active' (true/false) is required");

	IcingaApplication::GetInstance()->SetEnableNotifications(params->Get("active"));

	return ApiActions::CreateResult(200, "Globally " + String((params->Get("active") ? "enabled" : "disabled")) + " notifications.");
}

Dictionary::Ptr ApiActions::GlobalFlapDetection(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("active"))
		return ApiActions::CreateResult(403, "Parameter 'active' (true/false) is required");

	IcingaApplication::GetInstance()->SetEnableFlapping(params->Get("active"));

	return ApiActions::CreateResult(200, "Globally " + String((params->Get("active") ? "enabled" : "disabled")) + " flap detection.");
}

Dictionary::Ptr ApiActions::GlobalEventHandlers(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("active"))
		return ApiActions::CreateResult(403, "Parameter 'active' (true/false) is required");

	IcingaApplication::GetInstance()->SetEnableEventHandlers(params->Get("active"));

	return ApiActions::CreateResult(200, "Globally " + String((params->Get("active") ? "enabled" : "disabled")) + " event handlers.");
}

Dictionary::Ptr ApiActions::GlobalPerformanceData(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("active"))
		return ApiActions::CreateResult(403, "Parameter 'active' (true/false) is required");

	IcingaApplication::GetInstance()->SetEnablePerfdata(params->Get("active"));

	return ApiActions::CreateResult(200, "Globally " + String((params->Get("active") ? "enabled" : "disabled")) + " performance data processing.");
}

Dictionary::Ptr ApiActions::GlobalSvcCheckExecution(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("active"))
		return ApiActions::CreateResult(403, "Parameter 'active' (true/false) is required");

	IcingaApplication::GetInstance()->SetEnableServiceChecks(params->Get("active"));

	return ApiActions::CreateResult(200, "Globally " + String((params->Get("active") ? "enabled" : "disabled")) + " service checks.");
}

Dictionary::Ptr ApiActions::GlobalHostCheckExecution(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	if (!params->Contains("active"))
		return ApiActions::CreateResult(403, "Parameter 'active' (true/false) is required");

	IcingaApplication::GetInstance()->SetEnableHostChecks(params->Get("active"));

	return ApiActions::CreateResult(200, "Globally " + String((params->Get("active") ? "enabled" : "disabled")) + " host checks.");
}

//TODO-MA
/*
Dictionary::Ptr ApiActions::ChangeEventHandler(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot change event handler of a non-existent object");

   // empty command string implicitely disables event handler 
	if (!params->Contains("event_command_name")) {
		checkable->SetEnableEventHandler(false);
		return ApiActions::CreateResult(200, "Successfully disabled event handler for " + checkable->GetName());
	} else {
		String event_name = HttpUtility::GetLastParameter(params, "event_command_name");

		EventCommand::Ptr command = EventCommand::GetByName(event_name);

		if (!command)
			return ApiActions::CreateResult(404, "Event command '" + event_name + "' does not exist");

		checkable->SetEventCommand(command);

		return ApiActions::CreateResult(200, "Successfully changed event command for " + checkable->GetName());
	}
}

Dictionary::Ptr ApiActions::ChangeCheckCommand(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot change check command of a non-existent object");
	if (!params->Contains("check_command_name"))
		return ApiActions::CreateResult(403, "Parameter 'check_command_name' is required");

	String check_name = HttpUtility::GetLastParameter(params, "check_command_name");

	CheckCommand::Ptr command = CheckCommand::GetByName(check_name);

	if (!command)
		return ApiActions::CreateResult(404, "Check command '" + check_name + "' does not exist");

	checkable->SetCheckCommand(command);

	return ApiActions::CreateResult(200, "Successfully changed check command for " + checkable->GetName());
}

Dictionary::Ptr ApiActions::ChangeMaxCheckAttempts(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot change check command of a non-existent object");
	if (!params->Contains("max_check_attempts"))
		return ApiActions::CreateResult(403, "Parameter 'max_check_attempts' is required");

	checkable->SetMaxCheckAttempts(HttpUtility::GetLastParameter(params, "max_check_attempts"));

	return ApiActions::CreateResult(200, "Successfully changed the maximum check attempts for " + checkable->GetName());
}

Dictionary::Ptr ApiActions::ChangeCheckPeriod(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot change check command of a non-existent object");
	if (!params->Contains("time_period_name"))
		return ApiActions::CreateResult(403, "Parameter 'time_period_name' is required");

	String time_period_name = HttpUtility::GetLastParameter(params, "time_period_name");

	TimePeriod::Ptr time_period = TimePeriod::GetByName(time_period_name);

	if (!time_period)
		return ApiActions::CreateResult(404, "Time period '" + time_period_name + "' does not exist");

	checkable->SetCheckPeriod(time_period);

	return ApiActions::CreateResult(200, "Successfully changed the time period for " + checkable->GetName());
}

Dictionary::Ptr ApiActions::ChangeCheckInterval(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot change check command of a non-existent object");
	if (!params->Contains("check_interval"))
		return ApiActions::CreateResult(403, "Parameter 'check_interval' is required");

	checkable->SetCheckInterval(HttpUtility::GetLastParameter(params, "check_interval"));

	return ApiActions::CreateResult(200, "Successfully changed the check interval for " + checkable->GetName());
}

Dictionary::Ptr ApiActions::ChangeRetryInterval(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Checkable::Ptr checkable = static_pointer_cast<Checkable>(object);

	if (!checkable)
		return ApiActions::CreateResult(404, "Cannot change check command of a non-existent object");
	if (!params->Contains("retry_interval"))
		return ApiActions::CreateResult(403, "Parameter 'retry_interval' is required");

	checkable->SetRetryInterval(HttpUtility::GetLastParameter(params, "retry_interval"));

	return ApiActions::CreateResult(200, "Successfully changed the retry interval for " + checkable->GetName());
}
*/

Dictionary::Ptr ApiActions::ShutdownProcess(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Application::RequestShutdown();

	return ApiActions::CreateResult(200, "Shutting down Icinga2");
}

Dictionary::Ptr ApiActions::RestartProcess(const ConfigObject::Ptr& object, const Dictionary::Ptr& params)
{
	Application::RequestRestart();

	return ApiActions::CreateResult(200, "Restarting Icinga");
}

