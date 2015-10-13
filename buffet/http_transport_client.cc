// Copyright 2015 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "buffet/http_transport_client.h"

#include <base/bind.h>
#include <brillo/errors/error.h>
#include <brillo/http/http_request.h>
#include <brillo/http/http_utils.h>
#include <brillo/streams/memory_stream.h>
#include <weave/enum_to_string.h>

#include "buffet/weave_error_conversion.h"

namespace buffet {

namespace {

using weave::provider::HttpClient;

// The number of seconds each HTTP request will be allowed before timing out.
const int kRequestTimeoutSeconds = 30;

class ResponseImpl : public HttpClient::Response {
 public:
  ~ResponseImpl() override = default;
  explicit ResponseImpl(std::unique_ptr<brillo::http::Response> response)
      : response_{std::move(response)},
        data_{response_->ExtractDataAsString()} {}

  // HttpClient::Response implementation
  int GetStatusCode() const override { return response_->GetStatusCode(); }

  std::string GetContentType() const override {
    return response_->GetContentType();
  }

  std::string GetData() const override { return data_; }

 private:
  std::unique_ptr<brillo::http::Response> response_;
  std::string data_;
  DISALLOW_COPY_AND_ASSIGN(ResponseImpl);
};

void OnSuccessCallback(const HttpClient::SuccessCallback& success_callback,
                       int id,
                       std::unique_ptr<brillo::http::Response> response) {
  success_callback.Run(ResponseImpl{std::move(response)});
}

void OnErrorCallback(const weave::ErrorCallback& error_callback,
                     int id,
                     const brillo::Error* chromeos_error) {
  weave::ErrorPtr error;
  ConvertError(*chromeos_error, &error);
  error_callback.Run(std::move(error));
}

}  // anonymous namespace

HttpTransportClient::HttpTransportClient()
    : transport_{brillo::http::Transport::CreateDefault()} {
  transport_->SetDefaultTimeout(
      base::TimeDelta::FromSeconds(kRequestTimeoutSeconds));
}

HttpTransportClient::~HttpTransportClient() {}

void HttpTransportClient::SendRequest(
    Method method,
    const std::string& url,
    const Headers& headers,
    const std::string& data,
    const SuccessCallback& success_callback,
    const weave::ErrorCallback& error_callback) {
  brillo::http::Request request(url, weave::EnumToString(method), transport_);
  request.AddHeaders(headers);
  if (!data.empty()) {
    auto stream = brillo::MemoryStream::OpenCopyOf(data, nullptr);
    CHECK(stream->GetRemainingSize());
    brillo::ErrorPtr cromeos_error;
    if (!request.AddRequestBody(std::move(stream), &cromeos_error)) {
      weave::ErrorPtr error;
      ConvertError(*cromeos_error, &error);
      transport_->RunCallbackAsync(
          FROM_HERE, base::Bind(error_callback, base::Passed(&error)));
      return;
    }
  }
  request.GetResponse(base::Bind(&OnSuccessCallback, success_callback),
                      base::Bind(&OnErrorCallback, error_callback));
}

}  // namespace buffet
