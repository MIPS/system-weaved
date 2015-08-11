// Automatic generation of D-Bus interfaces:
//  - org.chromium.Buffet.Command
#ifndef ____CHROMEOS_DBUS_BINDING____________________BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_BUFFET_ORG_CHROMIUM_BUFFET_COMMAND_H
#define ____CHROMEOS_DBUS_BINDING____________________BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_BUFFET_ORG_CHROMIUM_BUFFET_COMMAND_H
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <base/macros.h>
#include <dbus/object_path.h>
#include <chromeos/any.h>
#include <chromeos/dbus/dbus_object.h>
#include <chromeos/dbus/exported_object_manager.h>
#include <chromeos/variant_dictionary.h>

namespace org {
namespace chromium {
namespace Buffet {

// Interface definition for org::chromium::Buffet::Command.
class CommandInterface {
 public:
  virtual ~CommandInterface() = default;

  virtual bool SetProgress(
      chromeos::ErrorPtr* error,
      const chromeos::VariantDictionary& in_progress) = 0;
  virtual bool SetResults(
      chromeos::ErrorPtr* error,
      const chromeos::VariantDictionary& in_results) = 0;
  // Mark the command as aborted. This tells the cloud that the device did
  // not successfully complete executing the command.
  virtual void Abort() = 0;
  // Mark the command as cancelled. Unlike Abort() this should be used when
  // the device detects a user request to cancel a command.
  virtual void Cancel() = 0;
  // Mark the command as successfully completed.
  virtual void Done() = 0;
};

// Interface adaptor for org::chromium::Buffet::Command.
class CommandAdaptor {
 public:
  CommandAdaptor(CommandInterface* interface) : interface_(interface) {}

  void RegisterWithDBusObject(chromeos::dbus_utils::DBusObject* object) {
    chromeos::dbus_utils::DBusInterface* itf =
        object->AddOrGetInterface("org.chromium.Buffet.Command");

    itf->AddSimpleMethodHandlerWithError(
        "SetProgress",
        base::Unretained(interface_),
        &CommandInterface::SetProgress);
    itf->AddSimpleMethodHandlerWithError(
        "SetResults",
        base::Unretained(interface_),
        &CommandInterface::SetResults);
    itf->AddSimpleMethodHandler(
        "Abort",
        base::Unretained(interface_),
        &CommandInterface::Abort);
    itf->AddSimpleMethodHandler(
        "Cancel",
        base::Unretained(interface_),
        &CommandInterface::Cancel);
    itf->AddSimpleMethodHandler(
        "Done",
        base::Unretained(interface_),
        &CommandInterface::Done);

    itf->AddProperty(NameName(), &name_);
    itf->AddProperty(CategoryName(), &category_);
    itf->AddProperty(IdName(), &id_);
    itf->AddProperty(StatusName(), &status_);
    itf->AddProperty(ParametersName(), &parameters_);
    itf->AddProperty(ProgressName(), &progress_);
    itf->AddProperty(ResultsName(), &results_);
    itf->AddProperty(OriginName(), &origin_);
  }

  static const char* NameName() { return "Name"; }
  std::string GetName() const {
    return name_.GetValue().Get<std::string>();
  }
  void SetName(const std::string& name) {
    name_.SetValue(name);
  }

  static const char* CategoryName() { return "Category"; }
  std::string GetCategory() const {
    return category_.GetValue().Get<std::string>();
  }
  void SetCategory(const std::string& category) {
    category_.SetValue(category);
  }

  static const char* IdName() { return "Id"; }
  std::string GetId() const {
    return id_.GetValue().Get<std::string>();
  }
  void SetId(const std::string& id) {
    id_.SetValue(id);
  }

  static const char* StatusName() { return "Status"; }
  std::string GetStatus() const {
    return status_.GetValue().Get<std::string>();
  }
  void SetStatus(const std::string& status) {
    status_.SetValue(status);
  }

  static const char* ParametersName() { return "Parameters"; }
  chromeos::VariantDictionary GetParameters() const {
    return parameters_.GetValue().Get<chromeos::VariantDictionary>();
  }
  void SetParameters(const chromeos::VariantDictionary& parameters) {
    parameters_.SetValue(parameters);
  }

  static const char* ProgressName() { return "Progress"; }
  chromeos::VariantDictionary GetProgress() const {
    return progress_.GetValue().Get<chromeos::VariantDictionary>();
  }
  void SetProgress(const chromeos::VariantDictionary& progress) {
    progress_.SetValue(progress);
  }

  static const char* ResultsName() { return "Results"; }
  chromeos::VariantDictionary GetResults() const {
    return results_.GetValue().Get<chromeos::VariantDictionary>();
  }
  void SetResults(const chromeos::VariantDictionary& results) {
    results_.SetValue(results);
  }

  // Specifies the origin of the command. This is a string containing
  // "cloud" or "local" indicating the method of delivery of the command.
  static const char* OriginName() { return "Origin"; }
  std::string GetOrigin() const {
    return origin_.GetValue().Get<std::string>();
  }
  void SetOrigin(const std::string& origin) {
    origin_.SetValue(origin);
  }

 private:
  chromeos::dbus_utils::ExportedProperty<std::string> name_;
  chromeos::dbus_utils::ExportedProperty<std::string> category_;
  chromeos::dbus_utils::ExportedProperty<std::string> id_;
  chromeos::dbus_utils::ExportedProperty<std::string> status_;
  chromeos::dbus_utils::ExportedProperty<chromeos::VariantDictionary> parameters_;
  chromeos::dbus_utils::ExportedProperty<chromeos::VariantDictionary> progress_;
  chromeos::dbus_utils::ExportedProperty<chromeos::VariantDictionary> results_;
  chromeos::dbus_utils::ExportedProperty<std::string> origin_;

  CommandInterface* interface_;  // Owned by container of this adapter.

  DISALLOW_COPY_AND_ASSIGN(CommandAdaptor);
};

}  // namespace Buffet
}  // namespace chromium
}  // namespace org
#endif  // ____CHROMEOS_DBUS_BINDING____________________BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_BUFFET_ORG_CHROMIUM_BUFFET_COMMAND_H
