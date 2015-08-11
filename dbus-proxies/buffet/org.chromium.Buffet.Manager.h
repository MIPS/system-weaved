// Automatic generation of D-Bus interfaces:
//  - org.chromium.Buffet.Manager
#ifndef ____CHROMEOS_DBUS_BINDING____________________BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_BUFFET_ORG_CHROMIUM_BUFFET_MANAGER_H
#define ____CHROMEOS_DBUS_BINDING____________________BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_BUFFET_ORG_CHROMIUM_BUFFET_MANAGER_H
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

// Interface definition for org::chromium::Buffet::Manager.
// The Manager is responsible for global state of Buffet.  It exposes
// interfaces which affect the entire device such as device registration and
// device state.
class ManagerInterface {
 public:
  virtual ~ManagerInterface() = default;

  virtual void CheckDeviceRegistered(
      std::unique_ptr<chromeos::dbus_utils::DBusMethodResponse<std::string>> response) = 0;
  virtual void GetDeviceInfo(
      std::unique_ptr<chromeos::dbus_utils::DBusMethodResponse<std::string>> response) = 0;
  virtual void RegisterDevice(
      std::unique_ptr<chromeos::dbus_utils::DBusMethodResponse<std::string>> response,
      const std::string& in_ticket_id) = 0;
  virtual bool UpdateDeviceInfo(
      chromeos::ErrorPtr* error,
      const std::string& in_name,
      const std::string& in_description,
      const std::string& in_location) = 0;
  virtual bool UpdateServiceConfig(
      chromeos::ErrorPtr* error,
      const std::string& in_client_id,
      const std::string& in_client_secret,
      const std::string& in_api_key,
      const std::string& in_oauth_url,
      const std::string& in_service_url) = 0;
  virtual void UpdateState(
      std::unique_ptr<chromeos::dbus_utils::DBusMethodResponse<>> response,
      const chromeos::VariantDictionary& in_property_set) = 0;
  virtual bool GetState(
      chromeos::ErrorPtr* error,
      std::string* out_device_info) = 0;
  virtual void AddCommand(
      std::unique_ptr<chromeos::dbus_utils::DBusMethodResponse<std::string>> response,
      const std::string& in_json_command,
      const std::string& in_user_role) = 0;
  virtual void GetCommand(
      std::unique_ptr<chromeos::dbus_utils::DBusMethodResponse<std::string>> response,
      const std::string& in_id) = 0;
  virtual std::string TestMethod(
      const std::string& in_message) = 0;
  // Enables WiFiBootstrapping if manual bootstrapping is selected via the
  // configuration file.  This will re-purpose a WiFi interface for use in
  // bootstrapping.  This breaks any existing WiFi connection on the
  // interface.
  virtual bool EnableWiFiBootstrapping(
      chromeos::ErrorPtr* error,
      const dbus::ObjectPath& in_listener_path,
      const chromeos::VariantDictionary& in_options) = 0;
  // If a previous call to EnableWiFiBootstrapping was successful and
  // has not been cancelled or completed since, disables that bootstrapping
  // process.
  virtual bool DisableWiFiBootstrapping(
      chromeos::ErrorPtr* error) = 0;
  virtual bool EnableGCDBootstrapping(
      chromeos::ErrorPtr* error,
      const dbus::ObjectPath& in_listener_path,
      const chromeos::VariantDictionary& in_options) = 0;
  virtual bool DisableGCDBootstrapping(
      chromeos::ErrorPtr* error) = 0;
};

// Interface adaptor for org::chromium::Buffet::Manager.
class ManagerAdaptor {
 public:
  ManagerAdaptor(ManagerInterface* interface) : interface_(interface) {}

  void RegisterWithDBusObject(chromeos::dbus_utils::DBusObject* object) {
    chromeos::dbus_utils::DBusInterface* itf =
        object->AddOrGetInterface("org.chromium.Buffet.Manager");

    itf->AddMethodHandler(
        "CheckDeviceRegistered",
        base::Unretained(interface_),
        &ManagerInterface::CheckDeviceRegistered);
    itf->AddMethodHandler(
        "GetDeviceInfo",
        base::Unretained(interface_),
        &ManagerInterface::GetDeviceInfo);
    itf->AddMethodHandler(
        "RegisterDevice",
        base::Unretained(interface_),
        &ManagerInterface::RegisterDevice);
    itf->AddSimpleMethodHandlerWithError(
        "UpdateDeviceInfo",
        base::Unretained(interface_),
        &ManagerInterface::UpdateDeviceInfo);
    itf->AddSimpleMethodHandlerWithError(
        "UpdateServiceConfig",
        base::Unretained(interface_),
        &ManagerInterface::UpdateServiceConfig);
    itf->AddMethodHandler(
        "UpdateState",
        base::Unretained(interface_),
        &ManagerInterface::UpdateState);
    itf->AddSimpleMethodHandlerWithError(
        "GetState",
        base::Unretained(interface_),
        &ManagerInterface::GetState);
    itf->AddMethodHandler(
        "AddCommand",
        base::Unretained(interface_),
        &ManagerInterface::AddCommand);
    itf->AddMethodHandler(
        "GetCommand",
        base::Unretained(interface_),
        &ManagerInterface::GetCommand);
    itf->AddSimpleMethodHandler(
        "TestMethod",
        base::Unretained(interface_),
        &ManagerInterface::TestMethod);
    itf->AddSimpleMethodHandlerWithError(
        "EnableWiFiBootstrapping",
        base::Unretained(interface_),
        &ManagerInterface::EnableWiFiBootstrapping);
    itf->AddSimpleMethodHandlerWithError(
        "DisableWiFiBootstrapping",
        base::Unretained(interface_),
        &ManagerInterface::DisableWiFiBootstrapping);
    itf->AddSimpleMethodHandlerWithError(
        "EnableGCDBootstrapping",
        base::Unretained(interface_),
        &ManagerInterface::EnableGCDBootstrapping);
    itf->AddSimpleMethodHandlerWithError(
        "DisableGCDBootstrapping",
        base::Unretained(interface_),
        &ManagerInterface::DisableGCDBootstrapping);

    itf->AddProperty(StatusName(), &status_);
    itf->AddProperty(DeviceIdName(), &device_id_);
    itf->AddProperty(StateName(), &state_);
    itf->AddProperty(OemNameName(), &oem_name_);
    itf->AddProperty(ModelNameName(), &model_name_);
    itf->AddProperty(ModelIdName(), &model_id_);
    itf->AddProperty(NameName(), &name_);
    itf->AddProperty(DescriptionName(), &description_);
    itf->AddProperty(LocationName(), &location_);
    itf->AddProperty(AnonymousAccessRoleName(), &anonymous_access_role_);
    itf->AddProperty(WiFiBootstrapStateName(), &wi_fi_bootstrap_state_);
    itf->AddProperty(GCDBootstrapStateName(), &gcdbootstrap_state_);
    itf->AddProperty(PairingInfoName(), &pairing_info_);
  }

  // State of Buffet's cloud registration.
  // Possible values include:
  //   "unconfigured": Buffet has no credentials, either from an out of box
  //                   state, or because device was unregistered.
  //
  //   "connecting": Buffet is registered and attempting to connect to the
  //                 cloud.
  //
  //   "connected": Buffet is online and connected to the cloud. Note that
  //                only this state requires internet connectivity.
  //
  //   "invalid_credentials": Buffet has credentials, but they are no longer
  //                          valid.
  static const char* StatusName() { return "Status"; }
  std::string GetStatus() const {
    return status_.GetValue().Get<std::string>();
  }
  void SetStatus(const std::string& status) {
    status_.SetValue(status);
  }

  // GCD ID if the device is registered or empty otherwise.
  static const char* DeviceIdName() { return "DeviceId"; }
  std::string GetDeviceId() const {
    return device_id_.GetValue().Get<std::string>();
  }
  void SetDeviceId(const std::string& device_id) {
    device_id_.SetValue(device_id);
  }

  // JSON with state of the devices.
  static const char* StateName() { return "State"; }
  std::string GetState() const {
    return state_.GetValue().Get<std::string>();
  }
  void SetState(const std::string& state) {
    state_.SetValue(state);
  }

  // Name of the device maker.
  static const char* OemNameName() { return "OemName"; }
  std::string GetOemName() const {
    return oem_name_.GetValue().Get<std::string>();
  }
  void SetOemName(const std::string& oem_name) {
    oem_name_.SetValue(oem_name);
  }

  // Name of the device model.
  static const char* ModelNameName() { return "ModelName"; }
  std::string GetModelName() const {
    return model_name_.GetValue().Get<std::string>();
  }
  void SetModelName(const std::string& model_name) {
    model_name_.SetValue(model_name);
  }

  // Five character code assigned by the cloud registry of device models.
  static const char* ModelIdName() { return "ModelId"; }
  std::string GetModelId() const {
    return model_id_.GetValue().Get<std::string>();
  }
  void SetModelId(const std::string& model_id) {
    model_id_.SetValue(model_id);
  }

  // Human readable name of the device. Must not be empty.
  static const char* NameName() { return "Name"; }
  std::string GetName() const {
    return name_.GetValue().Get<std::string>();
  }
  void SetName(const std::string& name) {
    name_.SetValue(name);
  }

  // Human readable description of the device.
  static const char* DescriptionName() { return "Description"; }
  std::string GetDescription() const {
    return description_.GetValue().Get<std::string>();
  }
  void SetDescription(const std::string& description) {
    description_.SetValue(description);
  }

  // Location of the device.
  static const char* LocationName() { return "Location"; }
  std::string GetLocation() const {
    return location_.GetValue().Get<std::string>();
  }
  void SetLocation(const std::string& location) {
    location_.SetValue(location);
  }

  // Max role granted to anonymous user when accessing device over the local
  // network.
  // Possible values include:
  //   "none": Device does not allow local access by unauthenticated users.
  //
  //   "viewer": Device allows everyone authenticated to access device.
  //
  //   "user": Device allows everyone authenticated as 'user' to access
  //           device.
  //
  //   "owner": Device allows everyone authenticated as 'owner' to access
  //           device.
  static const char* AnonymousAccessRoleName() { return "AnonymousAccessRole"; }
  std::string GetAnonymousAccessRole() const {
    return anonymous_access_role_.GetValue().Get<std::string>();
  }
  void SetAnonymousAccessRole(const std::string& anonymous_access_role) {
    anonymous_access_role_.SetValue(anonymous_access_role);
  }

  // Contains one of the following values describing the state of WiFi
  // bootstrapping:
  //   “disabled” - Bootstrapping has been disabled in the config file.
  //   “waiting” - buffet is waiting to receive WiFi credentials from
  //               a paired peer.
  //   “connecting” - buffet has received WiFi credentials, and is now
  //                  attempting to connect to a WiFi network.
  //   “monitoring” - buffet is monitoring our connectivity and will
  //                  re-enable bootstrapping if connectivity fails in
  //                  automatic mode.
  //
  // Note: more values may be added later to this list.
  static const char* WiFiBootstrapStateName() { return "WiFiBootstrapState"; }
  std::string GetWiFiBootstrapState() const {
    return wi_fi_bootstrap_state_.GetValue().Get<std::string>();
  }
  void SetWiFiBootstrapState(const std::string& wi_fi_bootstrap_state) {
    wi_fi_bootstrap_state_.SetValue(wi_fi_bootstrap_state);
  }

  // Contains one of the following values describing the state of GCD
  // bootstrapping:
  //   “disabled” - GCD registration has been disabled in the config file.
  //   “offline” - GCD registration is unknown because the device is offline.
  //   “connecting” - GCD registration is unknown because the device is still
  //                  connecting to the cloud.
  //   “waiting” - Waiting to be configured with GCD credentials.
  //   “registering” - Registering the device with the GCD servers.
  //   “online” - Device is online and registered with GCD servers.
  //
  // Note: more values may be added later to this list.
  //
  // Clients that wish to present a single linear bootstrapping flow to users
  // may treat GCD bootstrapping states as a suffix to WiFi bootstrapping
  // states.  If we have no cloud connectivity, we cannot possibly do GCD
  // registration/credential verification.
  static const char* GCDBootstrapStateName() { return "GCDBootstrapState"; }
  std::string GetGCDBootstrapState() const {
    return gcdbootstrap_state_.GetValue().Get<std::string>();
  }
  void SetGCDBootstrapState(const std::string& gcdbootstrap_state) {
    gcdbootstrap_state_.SetValue(gcdbootstrap_state);
  }

  // Describes the state of device pairing. While no pairing attempt is in
  // progress, this dictionary will be empty. When a client initiates a
  // pairing transaction via /privet/v3/pairing/start, dictionary will
  // contain the following keys:
  //   “sessionId” - ID of the pairing session; generated by device
  //   “pairingMode” - Selected type of pairing from /privet/v3/pairing/start
  //                   (e.g. “pinCode” or “embeddedCode”)
  //   “code” - The pin code or embedded code as appropriate to the
  //            “pairingMode” value.  See design document.
  //            This value will be a string.
  static const char* PairingInfoName() { return "PairingInfo"; }
  chromeos::VariantDictionary GetPairingInfo() const {
    return pairing_info_.GetValue().Get<chromeos::VariantDictionary>();
  }
  void SetPairingInfo(const chromeos::VariantDictionary& pairing_info) {
    pairing_info_.SetValue(pairing_info);
  }

  static dbus::ObjectPath GetObjectPath() {
    return dbus::ObjectPath{"/org/chromium/Buffet/Manager"};
  }

 private:
  chromeos::dbus_utils::ExportedProperty<std::string> status_;
  chromeos::dbus_utils::ExportedProperty<std::string> device_id_;
  chromeos::dbus_utils::ExportedProperty<std::string> state_;
  chromeos::dbus_utils::ExportedProperty<std::string> oem_name_;
  chromeos::dbus_utils::ExportedProperty<std::string> model_name_;
  chromeos::dbus_utils::ExportedProperty<std::string> model_id_;
  chromeos::dbus_utils::ExportedProperty<std::string> name_;
  chromeos::dbus_utils::ExportedProperty<std::string> description_;
  chromeos::dbus_utils::ExportedProperty<std::string> location_;
  chromeos::dbus_utils::ExportedProperty<std::string> anonymous_access_role_;
  chromeos::dbus_utils::ExportedProperty<std::string> wi_fi_bootstrap_state_;
  chromeos::dbus_utils::ExportedProperty<std::string> gcdbootstrap_state_;
  chromeos::dbus_utils::ExportedProperty<chromeos::VariantDictionary> pairing_info_;

  ManagerInterface* interface_;  // Owned by container of this adapter.

  DISALLOW_COPY_AND_ASSIGN(ManagerAdaptor);
};

}  // namespace Buffet
}  // namespace chromium
}  // namespace org
#endif  // ____CHROMEOS_DBUS_BINDING____________________BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_BUFFET_ORG_CHROMIUM_BUFFET_MANAGER_H
