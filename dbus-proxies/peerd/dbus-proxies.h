// Automatic generation of D-Bus interfaces:
//  - org.chromium.peerd.Manager
//  - org.chromium.peerd.Peer
//  - org.chromium.peerd.Service
#ifndef ____CHROMEOS_DBUS_BINDING___BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_PEERD_DBUS_PROXIES_H
#define ____CHROMEOS_DBUS_BINDING___BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_PEERD_DBUS_PROXIES_H
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
namespace peerd {
class ObjectManagerProxy;
}  // namespace peerd
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace peerd {

// Abstract interface proxy for org::chromium::peerd::Manager.
// The Manager is responsible for global state of peerd.  It exposes
// functionality affecting the entire device such as monitoring and
// local service advertisements.
class ManagerProxyInterface {
 public:
  virtual ~ManagerProxyInterface() = default;

  virtual bool StartMonitoring(
      const std::vector<std::string>& in_requested_technologies,
      const chromeos::VariantDictionary& in_options,
      std::string* out_monitoring_token,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void StartMonitoringAsync(
      const std::vector<std::string>& in_requested_technologies,
      const chromeos::VariantDictionary& in_options,
      const base::Callback<void(const std::string& /*monitoring_token*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool StopMonitoring(
      const std::string& in_monitoring_token,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void StopMonitoringAsync(
      const std::string& in_monitoring_token,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool ExposeService(
      const std::string& in_service_id,
      const std::map<std::string, std::string>& in_service_info,
      const chromeos::VariantDictionary& in_options,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void ExposeServiceAsync(
      const std::string& in_service_id,
      const std::map<std::string, std::string>& in_service_info,
      const chromeos::VariantDictionary& in_options,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool RemoveExposedService(
      const std::string& in_service_id,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void RemoveExposedServiceAsync(
      const std::string& in_service_id,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual bool Ping(
      std::string* out_message,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  virtual void PingAsync(
      const base::Callback<void(const std::string& /*message*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) = 0;

  static const char* MonitoredTechnologiesName() { return "MonitoredTechnologies"; }
  virtual const std::vector<std::string>& monitored_technologies() const = 0;
};

}  // namespace peerd
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace peerd {

// Interface proxy for org::chromium::peerd::Manager.
// The Manager is responsible for global state of peerd.  It exposes
// functionality affecting the entire device such as monitoring and
// local service advertisements.
class ManagerProxy final : public ManagerProxyInterface {
 public:
  class PropertySet : public dbus::PropertySet {
   public:
    PropertySet(dbus::ObjectProxy* object_proxy,
                const PropertyChangedCallback& callback)
        : dbus::PropertySet{object_proxy,
                            "org.chromium.peerd.Manager",
                            callback} {
      RegisterProperty(MonitoredTechnologiesName(), &monitored_technologies);
    }

    chromeos::dbus_utils::Property<std::vector<std::string>> monitored_technologies;

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

  bool StartMonitoring(
      const std::vector<std::string>& in_requested_technologies,
      const chromeos::VariantDictionary& in_options,
      std::string* out_monitoring_token,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.peerd.Manager",
        "StartMonitoring",
        error,
        in_requested_technologies,
        in_options);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error, out_monitoring_token);
  }

  void StartMonitoringAsync(
      const std::vector<std::string>& in_requested_technologies,
      const chromeos::VariantDictionary& in_options,
      const base::Callback<void(const std::string& /*monitoring_token*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.peerd.Manager",
        "StartMonitoring",
        success_callback,
        error_callback,
        in_requested_technologies,
        in_options);
  }

  bool StopMonitoring(
      const std::string& in_monitoring_token,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.peerd.Manager",
        "StopMonitoring",
        error,
        in_monitoring_token);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  void StopMonitoringAsync(
      const std::string& in_monitoring_token,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.peerd.Manager",
        "StopMonitoring",
        success_callback,
        error_callback,
        in_monitoring_token);
  }

  bool ExposeService(
      const std::string& in_service_id,
      const std::map<std::string, std::string>& in_service_info,
      const chromeos::VariantDictionary& in_options,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.peerd.Manager",
        "ExposeService",
        error,
        in_service_id,
        in_service_info,
        in_options);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  void ExposeServiceAsync(
      const std::string& in_service_id,
      const std::map<std::string, std::string>& in_service_info,
      const chromeos::VariantDictionary& in_options,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.peerd.Manager",
        "ExposeService",
        success_callback,
        error_callback,
        in_service_id,
        in_service_info,
        in_options);
  }

  bool RemoveExposedService(
      const std::string& in_service_id,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.peerd.Manager",
        "RemoveExposedService",
        error,
        in_service_id);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error);
  }

  void RemoveExposedServiceAsync(
      const std::string& in_service_id,
      const base::Callback<void()>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.peerd.Manager",
        "RemoveExposedService",
        success_callback,
        error_callback,
        in_service_id);
  }

  bool Ping(
      std::string* out_message,
      chromeos::ErrorPtr* error,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    auto response = chromeos::dbus_utils::CallMethodAndBlockWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.peerd.Manager",
        "Ping",
        error);
    return response && chromeos::dbus_utils::ExtractMethodCallResults(
        response.get(), error, out_message);
  }

  void PingAsync(
      const base::Callback<void(const std::string& /*message*/)>& success_callback,
      const base::Callback<void(chromeos::Error*)>& error_callback,
      int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT) override {
    chromeos::dbus_utils::CallMethodWithTimeout(
        timeout_ms,
        dbus_object_proxy_,
        "org.chromium.peerd.Manager",
        "Ping",
        success_callback,
        error_callback);
  }

  const std::vector<std::string>& monitored_technologies() const override {
    return property_set_->monitored_technologies.value();
  }

 private:
  void OnPropertyChanged(const std::string& property_name) {
    if (!on_property_changed_.is_null())
      on_property_changed_.Run(this, property_name);
  }

  scoped_refptr<dbus::Bus> bus_;
  const std::string service_name_{"org.chromium.peerd"};
  const dbus::ObjectPath object_path_{"/org/chromium/peerd/Manager"};
  PropertySet* property_set_;
  base::Callback<void(ManagerProxy*, const std::string&)> on_property_changed_;
  dbus::ObjectProxy* dbus_object_proxy_;

  friend class org::chromium::peerd::ObjectManagerProxy;
  DISALLOW_COPY_AND_ASSIGN(ManagerProxy);
};

}  // namespace peerd
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace peerd {

// Abstract interface proxy for org::chromium::peerd::Peer.
// Peers represent remote devices.  In addition to containing a
// unique identifier or the remote peer, and the time last seen,
// a peer object may have 0 or more services exposed by that peer.
// These services may be found at path_to_owning_peer/services/*.
class PeerProxyInterface {
 public:
  virtual ~PeerProxyInterface() = default;

  static const char* UUIDName() { return "UUID"; }
  virtual const std::string& uuid() const = 0;
  static const char* LastSeenName() { return "LastSeen"; }
  virtual uint64_t last_seen() const = 0;
};

}  // namespace peerd
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace peerd {

// Interface proxy for org::chromium::peerd::Peer.
// Peers represent remote devices.  In addition to containing a
// unique identifier or the remote peer, and the time last seen,
// a peer object may have 0 or more services exposed by that peer.
// These services may be found at path_to_owning_peer/services/*.
class PeerProxy final : public PeerProxyInterface {
 public:
  class PropertySet : public dbus::PropertySet {
   public:
    PropertySet(dbus::ObjectProxy* object_proxy,
                const PropertyChangedCallback& callback)
        : dbus::PropertySet{object_proxy,
                            "org.chromium.peerd.Peer",
                            callback} {
      RegisterProperty(UUIDName(), &uuid);
      RegisterProperty(LastSeenName(), &last_seen);
    }

    chromeos::dbus_utils::Property<std::string> uuid;
    chromeos::dbus_utils::Property<uint64_t> last_seen;

   private:
    DISALLOW_COPY_AND_ASSIGN(PropertySet);
  };

  PeerProxy(
      const scoped_refptr<dbus::Bus>& bus,
      const dbus::ObjectPath& object_path,
      PropertySet* property_set) :
          bus_{bus},
          object_path_{object_path},
          property_set_{property_set},
          dbus_object_proxy_{
              bus_->GetObjectProxy(service_name_, object_path_)} {
  }

  ~PeerProxy() override {
  }

  void ReleaseObjectProxy(const base::Closure& callback) {
    bus_->RemoveObjectProxy(service_name_, object_path_, callback);
  }

  const dbus::ObjectPath& GetObjectPath() const {
    return object_path_;
  }

  dbus::ObjectProxy* GetObjectProxy() const { return dbus_object_proxy_; }

  void SetPropertyChangedCallback(
      const base::Callback<void(PeerProxy*, const std::string&)>& callback) {
    on_property_changed_ = callback;
  }

  const PropertySet* GetProperties() const { return property_set_; }
  PropertySet* GetProperties() { return property_set_; }

  const std::string& uuid() const override {
    return property_set_->uuid.value();
  }

  uint64_t last_seen() const override {
    return property_set_->last_seen.value();
  }

 private:
  void OnPropertyChanged(const std::string& property_name) {
    if (!on_property_changed_.is_null())
      on_property_changed_.Run(this, property_name);
  }

  scoped_refptr<dbus::Bus> bus_;
  const std::string service_name_{"org.chromium.peerd"};
  dbus::ObjectPath object_path_;
  PropertySet* property_set_;
  base::Callback<void(PeerProxy*, const std::string&)> on_property_changed_;
  dbus::ObjectProxy* dbus_object_proxy_;

  friend class org::chromium::peerd::ObjectManagerProxy;
  DISALLOW_COPY_AND_ASSIGN(PeerProxy);
};

}  // namespace peerd
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace peerd {

// Abstract interface proxy for org::chromium::peerd::Service.
// Service objects represent a service exposed by device.  They have
// an associated service ID which will be unique per device, and
// and a map of metadata about a service.  Finally, services contain
// a list of IP addresses that a given service from a particular peer
// was discovered over.
class ServiceProxyInterface {
 public:
  virtual ~ServiceProxyInterface() = default;

  static const char* PeerIdName() { return "PeerId"; }
  virtual const std::string& peer_id() const = 0;
  static const char* ServiceIdName() { return "ServiceId"; }
  virtual const std::string& service_id() const = 0;
  static const char* ServiceInfoName() { return "ServiceInfo"; }
  virtual const std::map<std::string, std::string>& service_info() const = 0;
  static const char* IpInfosName() { return "IpInfos"; }
  virtual const std::vector<std::tuple<std::vector<uint8_t>, uint16_t>>& ip_infos() const = 0;
};

}  // namespace peerd
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace peerd {

// Interface proxy for org::chromium::peerd::Service.
// Service objects represent a service exposed by device.  They have
// an associated service ID which will be unique per device, and
// and a map of metadata about a service.  Finally, services contain
// a list of IP addresses that a given service from a particular peer
// was discovered over.
class ServiceProxy final : public ServiceProxyInterface {
 public:
  class PropertySet : public dbus::PropertySet {
   public:
    PropertySet(dbus::ObjectProxy* object_proxy,
                const PropertyChangedCallback& callback)
        : dbus::PropertySet{object_proxy,
                            "org.chromium.peerd.Service",
                            callback} {
      RegisterProperty(PeerIdName(), &peer_id);
      RegisterProperty(ServiceIdName(), &service_id);
      RegisterProperty(ServiceInfoName(), &service_info);
      RegisterProperty(IpInfosName(), &ip_infos);
    }

    chromeos::dbus_utils::Property<std::string> peer_id;
    chromeos::dbus_utils::Property<std::string> service_id;
    chromeos::dbus_utils::Property<std::map<std::string, std::string>> service_info;
    chromeos::dbus_utils::Property<std::vector<std::tuple<std::vector<uint8_t>, uint16_t>>> ip_infos;

   private:
    DISALLOW_COPY_AND_ASSIGN(PropertySet);
  };

  ServiceProxy(
      const scoped_refptr<dbus::Bus>& bus,
      const dbus::ObjectPath& object_path,
      PropertySet* property_set) :
          bus_{bus},
          object_path_{object_path},
          property_set_{property_set},
          dbus_object_proxy_{
              bus_->GetObjectProxy(service_name_, object_path_)} {
  }

  ~ServiceProxy() override {
  }

  void ReleaseObjectProxy(const base::Closure& callback) {
    bus_->RemoveObjectProxy(service_name_, object_path_, callback);
  }

  const dbus::ObjectPath& GetObjectPath() const {
    return object_path_;
  }

  dbus::ObjectProxy* GetObjectProxy() const { return dbus_object_proxy_; }

  void SetPropertyChangedCallback(
      const base::Callback<void(ServiceProxy*, const std::string&)>& callback) {
    on_property_changed_ = callback;
  }

  const PropertySet* GetProperties() const { return property_set_; }
  PropertySet* GetProperties() { return property_set_; }

  const std::string& peer_id() const override {
    return property_set_->peer_id.value();
  }

  const std::string& service_id() const override {
    return property_set_->service_id.value();
  }

  const std::map<std::string, std::string>& service_info() const override {
    return property_set_->service_info.value();
  }

  const std::vector<std::tuple<std::vector<uint8_t>, uint16_t>>& ip_infos() const override {
    return property_set_->ip_infos.value();
  }

 private:
  void OnPropertyChanged(const std::string& property_name) {
    if (!on_property_changed_.is_null())
      on_property_changed_.Run(this, property_name);
  }

  scoped_refptr<dbus::Bus> bus_;
  const std::string service_name_{"org.chromium.peerd"};
  dbus::ObjectPath object_path_;
  PropertySet* property_set_;
  base::Callback<void(ServiceProxy*, const std::string&)> on_property_changed_;
  dbus::ObjectProxy* dbus_object_proxy_;

  friend class org::chromium::peerd::ObjectManagerProxy;
  DISALLOW_COPY_AND_ASSIGN(ServiceProxy);
};

}  // namespace peerd
}  // namespace chromium
}  // namespace org

namespace org {
namespace chromium {
namespace peerd {

class ObjectManagerProxy : public dbus::ObjectManager::Interface {
 public:
  ObjectManagerProxy(const scoped_refptr<dbus::Bus>& bus)
      : bus_{bus},
        dbus_object_manager_{bus->GetObjectManager(
            "org.chromium.peerd",
            dbus::ObjectPath{"/org/chromium/peerd"})} {
    dbus_object_manager_->RegisterInterface("org.chromium.peerd.Manager", this);
    dbus_object_manager_->RegisterInterface("org.chromium.peerd.Peer", this);
    dbus_object_manager_->RegisterInterface("org.chromium.peerd.Service", this);
  }

  ~ObjectManagerProxy() override {
    dbus_object_manager_->UnregisterInterface("org.chromium.peerd.Manager");
    dbus_object_manager_->UnregisterInterface("org.chromium.peerd.Peer");
    dbus_object_manager_->UnregisterInterface("org.chromium.peerd.Service");
  }

  dbus::ObjectManager* GetObjectManagerProxy() const {
    return dbus_object_manager_;
  }

  org::chromium::peerd::ManagerProxy* GetManagerProxy() {
    if (manager_instances_.empty())
      return nullptr;
    return manager_instances_.begin()->second.get();
  }
  std::vector<org::chromium::peerd::ManagerProxy*> GetManagerInstances() const {
    std::vector<org::chromium::peerd::ManagerProxy*> values;
    values.reserve(manager_instances_.size());
    for (const auto& pair : manager_instances_)
      values.push_back(pair.second.get());
    return values;
  }
  void SetManagerAddedCallback(
      const base::Callback<void(org::chromium::peerd::ManagerProxy*)>& callback) {
    on_manager_added_ = callback;
  }
  void SetManagerRemovedCallback(
      const base::Callback<void(const dbus::ObjectPath&)>& callback) {
    on_manager_removed_ = callback;
  }

  org::chromium::peerd::PeerProxy* GetPeerProxy(
      const dbus::ObjectPath& object_path) {
    auto p = peer_instances_.find(object_path);
    if (p != peer_instances_.end())
      return p->second.get();
    return nullptr;
  }
  std::vector<org::chromium::peerd::PeerProxy*> GetPeerInstances() const {
    std::vector<org::chromium::peerd::PeerProxy*> values;
    values.reserve(peer_instances_.size());
    for (const auto& pair : peer_instances_)
      values.push_back(pair.second.get());
    return values;
  }
  void SetPeerAddedCallback(
      const base::Callback<void(org::chromium::peerd::PeerProxy*)>& callback) {
    on_peer_added_ = callback;
  }
  void SetPeerRemovedCallback(
      const base::Callback<void(const dbus::ObjectPath&)>& callback) {
    on_peer_removed_ = callback;
  }

  org::chromium::peerd::ServiceProxy* GetServiceProxy(
      const dbus::ObjectPath& object_path) {
    auto p = service_instances_.find(object_path);
    if (p != service_instances_.end())
      return p->second.get();
    return nullptr;
  }
  std::vector<org::chromium::peerd::ServiceProxy*> GetServiceInstances() const {
    std::vector<org::chromium::peerd::ServiceProxy*> values;
    values.reserve(service_instances_.size());
    for (const auto& pair : service_instances_)
      values.push_back(pair.second.get());
    return values;
  }
  void SetServiceAddedCallback(
      const base::Callback<void(org::chromium::peerd::ServiceProxy*)>& callback) {
    on_service_added_ = callback;
  }
  void SetServiceRemovedCallback(
      const base::Callback<void(const dbus::ObjectPath&)>& callback) {
    on_service_removed_ = callback;
  }

 private:
  void OnPropertyChanged(const dbus::ObjectPath& object_path,
                         const std::string& interface_name,
                         const std::string& property_name) {
    if (interface_name == "org.chromium.peerd.Manager") {
      auto p = manager_instances_.find(object_path);
      if (p == manager_instances_.end())
        return;
      p->second->OnPropertyChanged(property_name);
      return;
    }
    if (interface_name == "org.chromium.peerd.Peer") {
      auto p = peer_instances_.find(object_path);
      if (p == peer_instances_.end())
        return;
      p->second->OnPropertyChanged(property_name);
      return;
    }
    if (interface_name == "org.chromium.peerd.Service") {
      auto p = service_instances_.find(object_path);
      if (p == service_instances_.end())
        return;
      p->second->OnPropertyChanged(property_name);
      return;
    }
  }

  void ObjectAdded(
      const dbus::ObjectPath& object_path,
      const std::string& interface_name) override {
    if (interface_name == "org.chromium.peerd.Manager") {
      auto property_set =
          static_cast<org::chromium::peerd::ManagerProxy::PropertySet*>(
              dbus_object_manager_->GetProperties(object_path, interface_name));
      std::unique_ptr<org::chromium::peerd::ManagerProxy> manager_proxy{
        new org::chromium::peerd::ManagerProxy{bus_, property_set}
      };
      auto p = manager_instances_.emplace(object_path, std::move(manager_proxy));
      if (!on_manager_added_.is_null())
        on_manager_added_.Run(p.first->second.get());
      return;
    }
    if (interface_name == "org.chromium.peerd.Peer") {
      auto property_set =
          static_cast<org::chromium::peerd::PeerProxy::PropertySet*>(
              dbus_object_manager_->GetProperties(object_path, interface_name));
      std::unique_ptr<org::chromium::peerd::PeerProxy> peer_proxy{
        new org::chromium::peerd::PeerProxy{bus_, object_path, property_set}
      };
      auto p = peer_instances_.emplace(object_path, std::move(peer_proxy));
      if (!on_peer_added_.is_null())
        on_peer_added_.Run(p.first->second.get());
      return;
    }
    if (interface_name == "org.chromium.peerd.Service") {
      auto property_set =
          static_cast<org::chromium::peerd::ServiceProxy::PropertySet*>(
              dbus_object_manager_->GetProperties(object_path, interface_name));
      std::unique_ptr<org::chromium::peerd::ServiceProxy> service_proxy{
        new org::chromium::peerd::ServiceProxy{bus_, object_path, property_set}
      };
      auto p = service_instances_.emplace(object_path, std::move(service_proxy));
      if (!on_service_added_.is_null())
        on_service_added_.Run(p.first->second.get());
      return;
    }
  }

  void ObjectRemoved(
      const dbus::ObjectPath& object_path,
      const std::string& interface_name) override {
    if (interface_name == "org.chromium.peerd.Manager") {
      auto p = manager_instances_.find(object_path);
      if (p != manager_instances_.end()) {
        if (!on_manager_removed_.is_null())
          on_manager_removed_.Run(object_path);
        manager_instances_.erase(p);
      }
      return;
    }
    if (interface_name == "org.chromium.peerd.Peer") {
      auto p = peer_instances_.find(object_path);
      if (p != peer_instances_.end()) {
        if (!on_peer_removed_.is_null())
          on_peer_removed_.Run(object_path);
        peer_instances_.erase(p);
      }
      return;
    }
    if (interface_name == "org.chromium.peerd.Service") {
      auto p = service_instances_.find(object_path);
      if (p != service_instances_.end()) {
        if (!on_service_removed_.is_null())
          on_service_removed_.Run(object_path);
        service_instances_.erase(p);
      }
      return;
    }
  }

  dbus::PropertySet* CreateProperties(
      dbus::ObjectProxy* object_proxy,
      const dbus::ObjectPath& object_path,
      const std::string& interface_name) override {
    if (interface_name == "org.chromium.peerd.Manager") {
      return new org::chromium::peerd::ManagerProxy::PropertySet{
          object_proxy,
          base::Bind(&ObjectManagerProxy::OnPropertyChanged,
                     weak_ptr_factory_.GetWeakPtr(),
                     object_path,
                     interface_name)
      };
    }
    if (interface_name == "org.chromium.peerd.Peer") {
      return new org::chromium::peerd::PeerProxy::PropertySet{
          object_proxy,
          base::Bind(&ObjectManagerProxy::OnPropertyChanged,
                     weak_ptr_factory_.GetWeakPtr(),
                     object_path,
                     interface_name)
      };
    }
    if (interface_name == "org.chromium.peerd.Service") {
      return new org::chromium::peerd::ServiceProxy::PropertySet{
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
           std::unique_ptr<org::chromium::peerd::ManagerProxy>> manager_instances_;
  base::Callback<void(org::chromium::peerd::ManagerProxy*)> on_manager_added_;
  base::Callback<void(const dbus::ObjectPath&)> on_manager_removed_;
  std::map<dbus::ObjectPath,
           std::unique_ptr<org::chromium::peerd::PeerProxy>> peer_instances_;
  base::Callback<void(org::chromium::peerd::PeerProxy*)> on_peer_added_;
  base::Callback<void(const dbus::ObjectPath&)> on_peer_removed_;
  std::map<dbus::ObjectPath,
           std::unique_ptr<org::chromium::peerd::ServiceProxy>> service_instances_;
  base::Callback<void(org::chromium::peerd::ServiceProxy*)> on_service_added_;
  base::Callback<void(const dbus::ObjectPath&)> on_service_removed_;
  base::WeakPtrFactory<ObjectManagerProxy> weak_ptr_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(ObjectManagerProxy);
};

}  // namespace peerd
}  // namespace chromium
}  // namespace org

#endif  // ____CHROMEOS_DBUS_BINDING___BUILD_LINK_VAR_CACHE_PORTAGE_CHROMEOS_BASE_BUFFET_OUT_DEFAULT_GEN_INCLUDE_PEERD_DBUS_PROXIES_H
