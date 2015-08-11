// Automatic generation of D-Bus interfaces:
//  - org.chromium.Buffet.Command
//  - org.chromium.Buffet.Manager
#ifndef ____CHROMEOS_DBUS_BINDING___BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_BUFFET_DBUS_PROXIES_H
#define ____CHROMEOS_DBUS_BINDING___BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_BUFFET_DBUS_PROXIES_H
#include <memory>
#include <string>
#include <vector>

#include <base/bind.h>
#include <base/callback.h>
#include <base/logging.h>
#include <base/macros.h>
#include <base/memory/ref_counted.h>
#include <chromeos/any.h>
#include <chromeos/dbus/dbus_method_invoker.h>
#include <chromeos/dbus/dbus_property.h>
#include <chromeos/dbus/dbus_signal_handler.h>
#include <chromeos/errors/error.h>
#include <chromeos/variant_dictionary.h>
#include <dbus/bus.h>
#include <dbus/message.h>
#include <dbus/object_manager.h>
#include <dbus/object_path.h>
#include <dbus/object_proxy.h>

namespace org {
namespace chromium {
namespace Buffet {
class ObjectManagerProxy;
}  // namespace Buffet
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace Buffet {

// Abstract interface proxy for org::chromium::Buffet::Command.
class CommandProxyInterface {
 public:
  virtual ~CommandProxyInterface() = default;

  virtual bool SetProgress(
      const chromeos::VariantDictionary& in_progress,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void SetProgressAsync(
      const chromeos::VariantDictionary& in_progress,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool SetResults(
      const chromeos::VariantDictionary& in_results,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void SetResultsAsync(
      const chromeos::VariantDictionary& in_results,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  // Mark the command as aborted. This tells the cloud that the device did
  // not successfully complete executing the command.
  virtual bool Abort(
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  // Mark the command as aborted. This tells the cloud that the device did
  // not successfully complete executing the command.
  virtual void AbortAsync(
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  // Mark the command as cancelled. Unlike Abort() this should be used when
  // the device detects a user request to cancel a command.
  virtual bool Cancel(
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  // Mark the command as cancelled. Unlike Abort() this should be used when
  // the device detects a user request to cancel a command.
  virtual void CancelAsync(
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  // Mark the command as successfully completed.
  virtual bool Done(
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  // Mark the command as successfully completed.
  virtual void DoneAsync(
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  static const char* NameName() { return "Name"; }
  virtual const std::string& name() const = 0;
  static const char* CategoryName() { return "Category"; }
  virtual const std::string& category() const = 0;
  static const char* IdName() { return "Id"; }
  virtual const std::string& id() const = 0;
  static const char* StatusName() { return "Status"; }
  virtual const std::string& status() const = 0;
  static const char* ParametersName() { return "Parameters"; }
  virtual const chromeos::VariantDictionary& parameters() const = 0;
  static const char* ProgressName() { return "Progress"; }
  virtual const chromeos::VariantDictionary& progress() const = 0;
  static const char* ResultsName() { return "Results"; }
  virtual const chromeos::VariantDictionary& results() const = 0;
  static const char* OriginName() { return "Origin"; }
  virtual const std::string& origin() const = 0;
};

}  // namespace Buffet
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace Buffet {

// Interface proxy for org::chromium::Buffet::Command.
class CommandProxy final : public CommandProxyInterface {
 public:
  class PropertySet : public dbus::PropertySet {
   public:
    PropertySet(dbus::ObjectProxy* object_proxy,
                const PropertyChangedCallback& callback)
        : dbus::PropertySet{object_proxy,
                            "org.chromium.Buffet.Command",
                            callback} {
      RegisterProperty(NameName(), &name);
      RegisterProperty(CategoryName(), &category);
      RegisterProperty(IdName(), &id);
      RegisterProperty(StatusName(), &status);
      RegisterProperty(ParametersName(), &parameters);
      RegisterProperty(ProgressName(), &progress);
      RegisterProperty(ResultsName(), &results);
      RegisterProperty(OriginName(), &origin);
    }

    chromeos::dbus_utils::Property<std::string> name;
    chromeos::dbus_utils::Property<std::string> category;
    chromeos::dbus_utils::Property<std::string> id;
    chromeos::dbus_utils::Property<std::string> status;
    chromeos::dbus_utils::Property<chromeos::VariantDictionary> parameters;
    chromeos::dbus_utils::Property<chromeos::VariantDictionary> progress;
    chromeos::dbus_utils::Property<chromeos::VariantDictionary> results;
    chromeos::dbus_utils::Property<std::string> origin;

   private:
    DISALLOW_COPY_AND_ASSIGN(PropertySet);
  };

  CommandProxy(
      const scoped_refptr<dbus::Bus>& bus,
      const dbus::ObjectPath& object_path,
      PropertySet* property_set) :
          bus_{bus},
          object_path_{object_path},
          property_set_{property_set},
          dbus_object_proxy_{
              bus_->GetObjectProxy(service_name_, object_path_)} {
  }

  ~CommandProxy() override {
  }

  void ReleaseObjectProxy(const base::Closure& callback) {
    bus_->RemoveObjectProxy(service_name_, object_path_, callback);
  }

  const dbus::ObjectPath& GetObjectPath() const {
    return object_path_;
  }

  dbus::ObjectProxy* GetObjectProxy() const { return dbus_object_proxy_; }

  void SetPropertyChangedCallback(
      const base::Callback<void(CommandProxy*, const std::string&)>& callback) {
    on_property_changed_ = callback;
  }

  const PropertySet* GetProperties() const { return property_set_; }
  PropertySet* GetProperties() { return property_set_; }

  bool SetProgress(
      const chromeos::VariantDictionary& in_progress,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Command",
        "SetProgress",
        error,
        in_progress);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  void SetProgressAsync(
      const chromeos::VariantDictionary& in_progress,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Command",
        "SetProgress",
        success_callback,
        error_callback,
        in_progress);
  }

  bool SetResults(
      const chromeos::VariantDictionary& in_results,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Command",
        "SetResults",
        error,
        in_results);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  void SetResultsAsync(
      const chromeos::VariantDictionary& in_results,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Command",
        "SetResults",
        success_callback,
        error_callback,
        in_results);
  }

  // Mark the command as aborted. This tells the cloud that the device did
  // not successfully complete executing the command.
  bool Abort(
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Command",
        "Abort",
        error);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  // Mark the command as aborted. This tells the cloud that the device did
  // not successfully complete executing the command.
  void AbortAsync(
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Command",
        "Abort",
        success_callback,
        error_callback);
  }

  // Mark the command as cancelled. Unlike Abort() this should be used when
  // the device detects a user request to cancel a command.
  bool Cancel(
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Command",
        "Cancel",
        error);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  // Mark the command as cancelled. Unlike Abort() this should be used when
  // the device detects a user request to cancel a command.
  void CancelAsync(
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Command",
        "Cancel",
        success_callback,
        error_callback);
  }

  // Mark the command as successfully completed.
  bool Done(
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Command",
        "Done",
        error);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  // Mark the command as successfully completed.
  void DoneAsync(
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Command",
        "Done",
        success_callback,
        error_callback);
  }

  const std::string& name() const override {
    return property_set_->name.value();
  }

  const std::string& category() const override {
    return property_set_->category.value();
  }

  const std::string& id() const override {
    return property_set_->id.value();
  }

  const std::string& status() const override {
    return property_set_->status.value();
  }

  const chromeos::VariantDictionary& parameters() const override {
    return property_set_->parameters.value();
  }

  const chromeos::VariantDictionary& progress() const override {
    return property_set_->progress.value();
  }

  const chromeos::VariantDictionary& results() const override {
    return property_set_->results.value();
  }

  const std::string& origin() const override {
    return property_set_->origin.value();
  }

 private:
  void OnPropertyChanged(const std::string& property_name) {
    if (!on_property_changed_.is_null())
      on_property_changed_.Run(this, property_name);
  }

  scoped_refptr<dbus::Bus> bus_;
  const std::string service_name_{"org.chromium.Buffet"};
  dbus::ObjectPath object_path_;
  PropertySet* property_set_;
  base::Callback<void(CommandProxy*, const std::string&)> on_property_changed_;
  dbus::ObjectProxy* dbus_object_proxy_;

  friend class org::chromium::Buffet::ObjectManagerProxy;
  DISALLOW_COPY_AND_ASSIGN(CommandProxy);
};

}  // namespace Buffet
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace Buffet {

// Abstract interface proxy for org::chromium::Buffet::Manager.
// The Manager is responsible for global state of Buffet.  It exposes
// interfaces which affect the entire device such as device registration and
// device state.
class ManagerProxyInterface {
 public:
  virtual ~ManagerProxyInterface() = default;

  virtual bool CheckDeviceRegistered(
      std::string* out_device_id,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void CheckDeviceRegisteredAsync(
      const base::Callback<void(const std::string& /*device_id*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool GetDeviceInfo(
      std::string* out_device_info,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void GetDeviceInfoAsync(
      const base::Callback<void(const std::string& /*device_info*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool RegisterDevice(
      const std::string& in_ticket_id,
      std::string* out_device_id,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void RegisterDeviceAsync(
      const std::string& in_ticket_id,
      const base::Callback<void(const std::string& /*device_id*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool UpdateDeviceInfo(
      const std::string& in_name,
      const std::string& in_description,
      const std::string& in_location,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void UpdateDeviceInfoAsync(
      const std::string& in_name,
      const std::string& in_description,
      const std::string& in_location,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool UpdateServiceConfig(
      const std::string& in_client_id,
      const std::string& in_client_secret,
      const std::string& in_api_key,
      const std::string& in_oauth_url,
      const std::string& in_service_url,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void UpdateServiceConfigAsync(
      const std::string& in_client_id,
      const std::string& in_client_secret,
      const std::string& in_api_key,
      const std::string& in_oauth_url,
      const std::string& in_service_url,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool UpdateState(
      const chromeos::VariantDictionary& in_property_set,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void UpdateStateAsync(
      const chromeos::VariantDictionary& in_property_set,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool GetState(
      std::string* out_device_info,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void GetStateAsync(
      const base::Callback<void(const std::string& /*device_info*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool AddCommand(
      const std::string& in_json_command,
      const std::string& in_user_role,
      std::string* out_id,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void AddCommandAsync(
      const std::string& in_json_command,
      const std::string& in_user_role,
      const base::Callback<void(const std::string& /*id*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool GetCommand(
      const std::string& in_id,
      std::string* out_json_command,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void GetCommandAsync(
      const std::string& in_id,
      const base::Callback<void(const std::string& /*json_command*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool TestMethod(
      const std::string& in_message,
      std::string* out_echoed_message,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void TestMethodAsync(
      const std::string& in_message,
      const base::Callback<void(const std::string& /*echoed_message*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  // Enables WiFiBootstrapping if manual bootstrapping is selected via the
  // configuration file.  This will re-purpose a WiFi interface for use in
  // bootstrapping.  This breaks any existing WiFi connection on the
  // interface.
  virtual bool EnableWiFiBootstrapping(
      const dbus::ObjectPath& in_listener_path,
      const chromeos::VariantDictionary& in_options,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  // Enables WiFiBootstrapping if manual bootstrapping is selected via the
  // configuration file.  This will re-purpose a WiFi interface for use in
  // bootstrapping.  This breaks any existing WiFi connection on the
  // interface.
  virtual void EnableWiFiBootstrappingAsync(
      const dbus::ObjectPath& in_listener_path,
      const chromeos::VariantDictionary& in_options,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  // If a previous call to EnableWiFiBootstrapping was successful and
  // has not been cancelled or completed since, disables that bootstrapping
  // process.
  virtual bool DisableWiFiBootstrapping(
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  // If a previous call to EnableWiFiBootstrapping was successful and
  // has not been cancelled or completed since, disables that bootstrapping
  // process.
  virtual void DisableWiFiBootstrappingAsync(
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool EnableGCDBootstrapping(
      const dbus::ObjectPath& in_listener_path,
      const chromeos::VariantDictionary& in_options,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void EnableGCDBootstrappingAsync(
      const dbus::ObjectPath& in_listener_path,
      const chromeos::VariantDictionary& in_options,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool DisableGCDBootstrapping(
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void DisableGCDBootstrappingAsync(
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  static const char* StatusName() { return "Status"; }
  virtual const std::string& status() const = 0;
  static const char* DeviceIdName() { return "DeviceId"; }
  virtual const std::string& device_id() const = 0;
  static const char* StateName() { return "State"; }
  virtual const std::string& state() const = 0;
  static const char* OemNameName() { return "OemName"; }
  virtual const std::string& oem_name() const = 0;
  static const char* ModelNameName() { return "ModelName"; }
  virtual const std::string& model_name() const = 0;
  static const char* ModelIdName() { return "ModelId"; }
  virtual const std::string& model_id() const = 0;
  static const char* NameName() { return "Name"; }
  virtual const std::string& name() const = 0;
  static const char* DescriptionName() { return "Description"; }
  virtual const std::string& description() const = 0;
  static const char* LocationName() { return "Location"; }
  virtual const std::string& location() const = 0;
  static const char* AnonymousAccessRoleName() { return "AnonymousAccessRole"; }
  virtual const std::string& anonymous_access_role() const = 0;
  static const char* WiFiBootstrapStateName() { return "WiFiBootstrapState"; }
  virtual const std::string& wi_fi_bootstrap_state() const = 0;
  static const char* GCDBootstrapStateName() { return "GCDBootstrapState"; }
  virtual const std::string& gcdbootstrap_state() const = 0;
  static const char* PairingInfoName() { return "PairingInfo"; }
  virtual const chromeos::VariantDictionary& pairing_info() const = 0;
};

}  // namespace Buffet
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace Buffet {

// Interface proxy for org::chromium::Buffet::Manager.
// The Manager is responsible for global state of Buffet.  It exposes
// interfaces which affect the entire device such as device registration and
// device state.
class ManagerProxy final : public ManagerProxyInterface {
 public:
  class PropertySet : public dbus::PropertySet {
   public:
    PropertySet(dbus::ObjectProxy* object_proxy,
                const PropertyChangedCallback& callback)
        : dbus::PropertySet{object_proxy,
                            "org.chromium.Buffet.Manager",
                            callback} {
      RegisterProperty(StatusName(), &status);
      RegisterProperty(DeviceIdName(), &device_id);
      RegisterProperty(StateName(), &state);
      RegisterProperty(OemNameName(), &oem_name);
      RegisterProperty(ModelNameName(), &model_name);
      RegisterProperty(ModelIdName(), &model_id);
      RegisterProperty(NameName(), &name);
      RegisterProperty(DescriptionName(), &description);
      RegisterProperty(LocationName(), &location);
      RegisterProperty(AnonymousAccessRoleName(), &anonymous_access_role);
      RegisterProperty(WiFiBootstrapStateName(), &wi_fi_bootstrap_state);
      RegisterProperty(GCDBootstrapStateName(), &gcdbootstrap_state);
      RegisterProperty(PairingInfoName(), &pairing_info);
    }

    chromeos::dbus_utils::Property<std::string> status;
    chromeos::dbus_utils::Property<std::string> device_id;
    chromeos::dbus_utils::Property<std::string> state;
    chromeos::dbus_utils::Property<std::string> oem_name;
    chromeos::dbus_utils::Property<std::string> model_name;
    chromeos::dbus_utils::Property<std::string> model_id;
    chromeos::dbus_utils::Property<std::string> name;
    chromeos::dbus_utils::Property<std::string> description;
    chromeos::dbus_utils::Property<std::string> location;
    chromeos::dbus_utils::Property<std::string> anonymous_access_role;
    chromeos::dbus_utils::Property<std::string> wi_fi_bootstrap_state;
    chromeos::dbus_utils::Property<std::string> gcdbootstrap_state;
    chromeos::dbus_utils::Property<chromeos::VariantDictionary> pairing_info;

   private:
    DISALLOW_COPY_AND_ASSIGN(PropertySet);
  };

  ManagerProxy(
      const scoped_refptr<dbus::Bus>& bus,
      PropertySet* property_set) :
          bus_{bus},
          property_set_{property_set},
          dbus_object_proxy_{
              bus_->GetObjectProxy(service_name_, object_path_)} {
  }

  ~ManagerProxy() override {
  }

  void ReleaseObjectProxy(const base::Closure& callback) {
    bus_->RemoveObjectProxy(service_name_, object_path_, callback);
  }

  const dbus::ObjectPath& GetObjectPath() const {
    return object_path_;
  }

  dbus::ObjectProxy* GetObjectProxy() const { return dbus_object_proxy_; }

  void SetPropertyChangedCallback(
      const base::Callback<void(ManagerProxy*, const std::string&)>& callback) {
    on_property_changed_ = callback;
  }

  const PropertySet* GetProperties() const { return property_set_; }
  PropertySet* GetProperties() { return property_set_; }

  bool CheckDeviceRegistered(
      std::string* out_device_id,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "CheckDeviceRegistered",
        error);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error, out_device_id);
  }

  void CheckDeviceRegisteredAsync(
      const base::Callback<void(const std::string& /*device_id*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "CheckDeviceRegistered",
        success_callback,
        error_callback);
  }

  bool GetDeviceInfo(
      std::string* out_device_info,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "GetDeviceInfo",
        error);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error, out_device_info);
  }

  void GetDeviceInfoAsync(
      const base::Callback<void(const std::string& /*device_info*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "GetDeviceInfo",
        success_callback,
        error_callback);
  }

  bool RegisterDevice(
      const std::string& in_ticket_id,
      std::string* out_device_id,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "RegisterDevice",
        error,
        in_ticket_id);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error, out_device_id);
  }

  void RegisterDeviceAsync(
      const std::string& in_ticket_id,
      const base::Callback<void(const std::string& /*device_id*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "RegisterDevice",
        success_callback,
        error_callback,
        in_ticket_id);
  }

  bool UpdateDeviceInfo(
      const std::string& in_name,
      const std::string& in_description,
      const std::string& in_location,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "UpdateDeviceInfo",
        error,
        in_name,
        in_description,
        in_location);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  void UpdateDeviceInfoAsync(
      const std::string& in_name,
      const std::string& in_description,
      const std::string& in_location,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "UpdateDeviceInfo",
        success_callback,
        error_callback,
        in_name,
        in_description,
        in_location);
  }

  bool UpdateServiceConfig(
      const std::string& in_client_id,
      const std::string& in_client_secret,
      const std::string& in_api_key,
      const std::string& in_oauth_url,
      const std::string& in_service_url,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "UpdateServiceConfig",
        error,
        in_client_id,
        in_client_secret,
        in_api_key,
        in_oauth_url,
        in_service_url);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  void UpdateServiceConfigAsync(
      const std::string& in_client_id,
      const std::string& in_client_secret,
      const std::string& in_api_key,
      const std::string& in_oauth_url,
      const std::string& in_service_url,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "UpdateServiceConfig",
        success_callback,
        error_callback,
        in_client_id,
        in_client_secret,
        in_api_key,
        in_oauth_url,
        in_service_url);
  }

  bool UpdateState(
      const chromeos::VariantDictionary& in_property_set,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "UpdateState",
        error,
        in_property_set);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  void UpdateStateAsync(
      const chromeos::VariantDictionary& in_property_set,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "UpdateState",
        success_callback,
        error_callback,
        in_property_set);
  }

  bool GetState(
      std::string* out_device_info,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "GetState",
        error);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error, out_device_info);
  }

  void GetStateAsync(
      const base::Callback<void(const std::string& /*device_info*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "GetState",
        success_callback,
        error_callback);
  }

  bool AddCommand(
      const std::string& in_json_command,
      const std::string& in_user_role,
      std::string* out_id,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "AddCommand",
        error,
        in_json_command,
        in_user_role);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error, out_id);
  }

  void AddCommandAsync(
      const std::string& in_json_command,
      const std::string& in_user_role,
      const base::Callback<void(const std::string& /*id*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "AddCommand",
        success_callback,
        error_callback,
        in_json_command,
        in_user_role);
  }

  bool GetCommand(
      const std::string& in_id,
      std::string* out_json_command,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "GetCommand",
        error,
        in_id);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error, out_json_command);
  }

  void GetCommandAsync(
      const std::string& in_id,
      const base::Callback<void(const std::string& /*json_command*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "GetCommand",
        success_callback,
        error_callback,
        in_id);
  }

  bool TestMethod(
      const std::string& in_message,
      std::string* out_echoed_message,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "TestMethod",
        error,
        in_message);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error, out_echoed_message);
  }

  void TestMethodAsync(
      const std::string& in_message,
      const base::Callback<void(const std::string& /*echoed_message*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "TestMethod",
        success_callback,
        error_callback,
        in_message);
  }

  // Enables WiFiBootstrapping if manual bootstrapping is selected via the
  // configuration file.  This will re-purpose a WiFi interface for use in
  // bootstrapping.  This breaks any existing WiFi connection on the
  // interface.
  bool EnableWiFiBootstrapping(
      const dbus::ObjectPath& in_listener_path,
      const chromeos::VariantDictionary& in_options,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "EnableWiFiBootstrapping",
        error,
        in_listener_path,
        in_options);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  // Enables WiFiBootstrapping if manual bootstrapping is selected via the
  // configuration file.  This will re-purpose a WiFi interface for use in
  // bootstrapping.  This breaks any existing WiFi connection on the
  // interface.
  void EnableWiFiBootstrappingAsync(
      const dbus::ObjectPath& in_listener_path,
      const chromeos::VariantDictionary& in_options,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "EnableWiFiBootstrapping",
        success_callback,
        error_callback,
        in_listener_path,
        in_options);
  }

  // If a previous call to EnableWiFiBootstrapping was successful and
  // has not been cancelled or completed since, disables that bootstrapping
  // process.
  bool DisableWiFiBootstrapping(
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "DisableWiFiBootstrapping",
        error);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  // If a previous call to EnableWiFiBootstrapping was successful and
  // has not been cancelled or completed since, disables that bootstrapping
  // process.
  void DisableWiFiBootstrappingAsync(
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "DisableWiFiBootstrapping",
        success_callback,
        error_callback);
  }

  bool EnableGCDBootstrapping(
      const dbus::ObjectPath& in_listener_path,
      const chromeos::VariantDictionary& in_options,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "EnableGCDBootstrapping",
        error,
        in_listener_path,
        in_options);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  void EnableGCDBootstrappingAsync(
      const dbus::ObjectPath& in_listener_path,
      const chromeos::VariantDictionary& in_options,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "EnableGCDBootstrapping",
        success_callback,
        error_callback,
        in_listener_path,
        in_options);
  }

  bool DisableGCDBootstrapping(
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "DisableGCDBootstrapping",
        error);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  void DisableGCDBootstrappingAsync(
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.Buffet.Manager",
        "DisableGCDBootstrapping",
        success_callback,
        error_callback);
  }

  const std::string& status() const override {
    return property_set_->status.value();
  }

  const std::string& device_id() const override {
    return property_set_->device_id.value();
  }

  const std::string& state() const override {
    return property_set_->state.value();
  }

  const std::string& oem_name() const override {
    return property_set_->oem_name.value();
  }

  const std::string& model_name() const override {
    return property_set_->model_name.value();
  }

  const std::string& model_id() const override {
    return property_set_->model_id.value();
  }

  const std::string& name() const override {
    return property_set_->name.value();
  }

  const std::string& description() const override {
    return property_set_->description.value();
  }

  const std::string& location() const override {
    return property_set_->location.value();
  }

  const std::string& anonymous_access_role() const override {
    return property_set_->anonymous_access_role.value();
  }

  const std::string& wi_fi_bootstrap_state() const override {
    return property_set_->wi_fi_bootstrap_state.value();
  }

  const std::string& gcdbootstrap_state() const override {
    return property_set_->gcdbootstrap_state.value();
  }

  const chromeos::VariantDictionary& pairing_info() const override {
    return property_set_->pairing_info.value();
  }

 private:
  void OnPropertyChanged(const std::string& property_name) {
    if (!on_property_changed_.is_null())
      on_property_changed_.Run(this, property_name);
  }

  scoped_refptr<dbus::Bus> bus_;
  const std::string service_name_{"org.chromium.Buffet"};
  const dbus::ObjectPath object_path_{"/org/chromium/Buffet/Manager"};
  PropertySet* property_set_;
  base::Callback<void(ManagerProxy*, const std::string&)> on_property_changed_;
  dbus::ObjectProxy* dbus_object_proxy_;

  friend class org::chromium::Buffet::ObjectManagerProxy;
  DISALLOW_COPY_AND_ASSIGN(ManagerProxy);
};

}  // namespace Buffet
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace Buffet {

class ObjectManagerProxy : public dbus::ObjectManager::Interface {
 public:
  ObjectManagerProxy(const scoped_refptr<dbus::Bus>& bus)
      : bus_{bus},
        dbus_object_manager_{bus->GetObjectManager(
            "org.chromium.Buffet",
            dbus::ObjectPath{"/org/chromium/Buffet"})} {
    dbus_object_manager_->RegisterInterface("org.chromium.Buffet.Command", this);
    dbus_object_manager_->RegisterInterface("org.chromium.Buffet.Manager", this);
  }

  ~ObjectManagerProxy() override {
    dbus_object_manager_->UnregisterInterface("org.chromium.Buffet.Command");
    dbus_object_manager_->UnregisterInterface("org.chromium.Buffet.Manager");
  }

  dbus::ObjectManager* GetObjectManagerProxy() const {
    return dbus_object_manager_;
  }

  org::chromium::Buffet::CommandProxy* GetCommandProxy(
      const dbus::ObjectPath& object_path) {
    auto p = command_instances_.find(object_path);
    if (p != command_instances_.end())
      return p->second.get();
    return nullptr;
  }
  std::vector<org::chromium::Buffet::CommandProxy*> GetCommandInstances() const {
    std::vector<org::chromium::Buffet::CommandProxy*> values;
    values.reserve(command_instances_.size());
    for (const auto& pair : command_instances_)
      values.push_back(pair.second.get());
    return values;
  }
  void SetCommandAddedCallback(
      const base::Callback<void(org::chromium::Buffet::CommandProxy*)>& callback) {
    on_command_added_ = callback;
  }
  void SetCommandRemovedCallback(
      const base::Callback<void(const dbus::ObjectPath&)>& callback) {
    on_command_removed_ = callback;
  }

  org::chromium::Buffet::ManagerProxy* GetManagerProxy() {
    if (manager_instances_.empty())
      return nullptr;
    return manager_instances_.begin()->second.get();
  }
  std::vector<org::chromium::Buffet::ManagerProxy*> GetManagerInstances() const {
    std::vector<org::chromium::Buffet::ManagerProxy*> values;
    values.reserve(manager_instances_.size());
    for (const auto& pair : manager_instances_)
      values.push_back(pair.second.get());
    return values;
  }
  void SetManagerAddedCallback(
      const base::Callback<void(org::chromium::Buffet::ManagerProxy*)>& callback) {
    on_manager_added_ = callback;
  }
  void SetManagerRemovedCallback(
      const base::Callback<void(const dbus::ObjectPath&)>& callback) {
    on_manager_removed_ = callback;
  }

 private:
  void OnPropertyChanged(const dbus::ObjectPath& object_path,
                         const std::string& interface_name,
                         const std::string& property_name) {
    if (interface_name == "org.chromium.Buffet.Command") {
      auto p = command_instances_.find(object_path);
      if (p == command_instances_.end())
        return;
      p->second->OnPropertyChanged(property_name);
      return;
    }
    if (interface_name == "org.chromium.Buffet.Manager") {
      auto p = manager_instances_.find(object_path);
      if (p == manager_instances_.end())
        return;
      p->second->OnPropertyChanged(property_name);
      return;
    }
  }

  void ObjectAdded(
      const dbus::ObjectPath& object_path,
      const std::string& interface_name) override {
    if (interface_name == "org.chromium.Buffet.Command") {
      auto property_set =
          static_cast<org::chromium::Buffet::CommandProxy::PropertySet*>(
              dbus_object_manager_->GetProperties(object_path, interface_name));
      std::unique_ptr<org::chromium::Buffet::CommandProxy> command_proxy{
        new org::chromium::Buffet::CommandProxy{bus_, object_path, property_set}
      };
      auto p = command_instances_.emplace(object_path, std::move(command_proxy));
      if (!on_command_added_.is_null())
        on_command_added_.Run(p.first->second.get());
      return;
    }
    if (interface_name == "org.chromium.Buffet.Manager") {
      auto property_set =
          static_cast<org::chromium::Buffet::ManagerProxy::PropertySet*>(
              dbus_object_manager_->GetProperties(object_path, interface_name));
      std::unique_ptr<org::chromium::Buffet::ManagerProxy> manager_proxy{
        new org::chromium::Buffet::ManagerProxy{bus_, property_set}
      };
      auto p = manager_instances_.emplace(object_path, std::move(manager_proxy));
      if (!on_manager_added_.is_null())
        on_manager_added_.Run(p.first->second.get());
      return;
    }
  }

  void ObjectRemoved(
      const dbus::ObjectPath& object_path,
      const std::string& interface_name) override {
    if (interface_name == "org.chromium.Buffet.Command") {
      auto p = command_instances_.find(object_path);
      if (p != command_instances_.end()) {
        if (!on_command_removed_.is_null())
          on_command_removed_.Run(object_path);
        command_instances_.erase(p);
      }
      return;
    }
    if (interface_name == "org.chromium.Buffet.Manager") {
      auto p = manager_instances_.find(object_path);
      if (p != manager_instances_.end()) {
        if (!on_manager_removed_.is_null())
          on_manager_removed_.Run(object_path);
        manager_instances_.erase(p);
      }
      return;
    }
  }

  dbus::PropertySet* CreateProperties(
      dbus::ObjectProxy* object_proxy,
      const dbus::ObjectPath& object_path,
      const std::string& interface_name) override {
    if (interface_name == "org.chromium.Buffet.Command") {
      return new org::chromium::Buffet::CommandProxy::PropertySet{
          object_proxy,
          base::Bind(&ObjectManagerProxy::OnPropertyChanged,
                     weak_ptr_factory_.GetWeakPtr(),
                     object_path,
                     interface_name)
      };
    }
    if (interface_name == "org.chromium.Buffet.Manager") {
      return new org::chromium::Buffet::ManagerProxy::PropertySet{
          object_proxy,
          base::Bind(&ObjectManagerProxy::OnPropertyChanged,
                     weak_ptr_factory_.GetWeakPtr(),
                     object_path,
                     interface_name)
      };
    }
    LOG(FATAL) << "Creating properties for unsupported interface "
               << interface_name;
    return nullptr;
  }

  scoped_refptr<dbus::Bus> bus_;
  dbus::ObjectManager* dbus_object_manager_;
  std::map<dbus::ObjectPath,
           std::unique_ptr<org::chromium::Buffet::CommandProxy>> command_instances_;
  base::Callback<void(org::chromium::Buffet::CommandProxy*)> on_command_added_;
  base::Callback<void(const dbus::ObjectPath&)> on_command_removed_;
  std::map<dbus::ObjectPath,
           std::unique_ptr<org::chromium::Buffet::ManagerProxy>> manager_instances_;
  base::Callback<void(org::chromium::Buffet::ManagerProxy*)> on_manager_added_;
  base::Callback<void(const dbus::ObjectPath&)> on_manager_removed_;
  base::WeakPtrFactory<ObjectManagerProxy> weak_ptr_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(ObjectManagerProxy);
};

}  // namespace Buffet
}  // namespace chromium
}  // namespace org

#endif  // ____CHROMEOS_DBUS_BINDING___BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_BUFFET_DBUS_PROXIES_H
