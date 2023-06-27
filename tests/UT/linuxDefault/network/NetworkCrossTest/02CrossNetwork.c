/**
****************************************************************************************
* @FilePath: FiscobcosNetwork_test.c
* @Author: aitos
* @Date: 2022-10-29 14:57:56
* @LastEditors:
* @LastEditTime: 2022-10-29 14:57:56
* @Descripttion:
****************************************************************************************
*/
/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#include "tcase_network.h"
#include "boatiotsdk.h"
#include "boatlog.h"

const BCHAR *demoUrl = "http://127.0.0.1:7545";

const BUINT32 chainID = 1;

const BCHAR *fabric_client_demokey = "-----BEGIN PRIVATE KEY-----\n"
                                     "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQg/tXXUwnCl5b0Un9q\n"
                                     "KjexHujDnmqxN2F4PkyUVjAlYBShRANCAAS84e5YxBu7VISBc5O8dVUaQ5EVV6KO\n"
                                     "84YIaQi7J64Te5gz8nSHM5jMPLeDQv6r0YknVKE3gl4BRvIWtDbxI3tI\n"
                                     "-----END PRIVATE KEY-----\n";

const BCHAR *fabric_client_democert = "-----BEGIN CERTIFICATE-----\n"
                                      "MIICKzCCAdGgAwIBAgIRAJVvD4lgmBO3Ov7TYwDyIhMwCgYIKoZIzj0EAwIwczEL\n"
                                      "MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
                                      "cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh\n"
                                      "Lm9yZzEuZXhhbXBsZS5jb20wHhcNMjIwNTA2MDMwMTAwWhcNMzIwNTAzMDMwMTAw\n"
                                      "WjBsMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN\n"
                                      "U2FuIEZyYW5jaXNjbzEPMA0GA1UECxMGY2xpZW50MR8wHQYDVQQDDBZVc2VyMUBv\n"
                                      "cmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEvOHuWMQb\n"
                                      "u1SEgXOTvHVVGkORFVeijvOGCGkIuyeuE3uYM/J0hzOYzDy3g0L+q9GJJ1ShN4Je\n"
                                      "AUbyFrQ28SN7SKNNMEswDgYDVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYD\n"
                                      "VR0jBCQwIoAgxfxkzp5erq3p4AH/lMgNe44f0BqYmWGuB4hXIkG540UwCgYIKoZI\n"
                                      "zj0EAwIDSAAwRQIhAOz8ruXrkkdK2EsfGrmr7paTnBhjwvbSx0GQXZk0CpJCAiBu\n"
                                      "wFbiCN1EwHqROzJgGMiatSOARqndHJpeX03w0qeSew==\n"
                                      "-----END CERTIFICATE-----\n";

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)

const BCHAR *fabric_client_tls_prikey = "-----BEGIN PRIVATE KEY-----\n"
                                        "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgHFtvXWryUMKKUHKJ\n"
                                        "3ajXvSG1aPvZnCvC1m57KjlSh1qhRANCAARi2dKHWoicjhXDeX/oGgMzh1oimvJy\n"
                                        "TZXp4b3coeZfPcmxrWJEU4ZbVvYOKxOfp7LnYyBc8ct/KM37ultOD7zK\n"
                                        "-----END PRIVATE KEY-----\n";

const BCHAR *fabric_client_tls_cert = "-----BEGIN CERTIFICATE-----\n"
                                      "MIICOzCCAeGgAwIBAgIQfuVAt3+dJm873ZX/by2dpTAKBggqhkjOPQQDAjB2MQsw\n"
                                      "CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
                                      "YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
                                      "Y2Eub3JnMS5leGFtcGxlLmNvbTAeFw0yMjA1MDYwMzAxMDBaFw0zMjA1MDMwMzAx\n"
                                      "MDBaMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
                                      "Ew1TYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZVc2VyMUBvcmcxLmV4YW1wbGUuY29t\n"
                                      "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEYtnSh1qInI4Vw3l/6BoDM4daIpry\n"
                                      "ck2V6eG93KHmXz3Jsa1iRFOGW1b2DisTn6ey52MgXPHLfyjN+7pbTg+8yqNsMGow\n"
                                      "DgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAM\n"
                                      "BgNVHRMBAf8EAjAAMCsGA1UdIwQkMCKAIBMm4MmGOC/2t1UTvnVvf5St1fbxyMam\n"
                                      "CoUPXIdX0BVNMAoGCCqGSM49BAMCA0gAMEUCIQDJTu1jVW3ZqOqXq7B4vPMHdN5s\n"
                                      "FYaxtCge3bdCgkjcNQIgRs1rFgxlFq7bkloGBucO/H43sG4wX4MDK4V4IfQVUkY=\n"
                                      "-----END CERTIFICATE-----\n";

#endif

const BCHAR *fabric_org1_tlsCert = "-----BEGIN CERTIFICATE-----\n"
                                   "MIICVzCCAf6gAwIBAgIRAOgZn4I4jGSdGbBD4m2VoVUwCgYIKoZIzj0EAwIwdjEL\n"
                                   "MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\n"
                                   "cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHzAdBgNVBAMTFnRs\n"
                                   "c2NhLm9yZzEuZXhhbXBsZS5jb20wHhcNMjIwNTA2MDMwMTAwWhcNMzIwNTAzMDMw\n"
                                   "MTAwWjB2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UE\n"
                                   "BxMNU2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0G\n"
                                   "A1UEAxMWdGxzY2Eub3JnMS5leGFtcGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49\n"
                                   "AwEHA0IABEtpvo/YvWG6ZlSmyE43LUNZZtgTg2B1LOKi2rJ+/6exBOiIHu0veF2D\n"
                                   "e+bLinBR5FGGzGAk6TV7sfLsc4yUpACjbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNV\n"
                                   "HSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNV\n"
                                   "HQ4EIgQgEybgyYY4L/a3VRO+dW9/lK3V9vHIxqYKhQ9ch1fQFU0wCgYIKoZIzj0E\n"
                                   "AwIDRwAwRAIgHJayi2TyzJUd4ofYC1i+asm9ar5dOO0x4E+xCCUupIUCIFMJCg0Z\n"
                                   "Yos13DpaCNoqNwAGJUcqLv4rtZPSPC4tkovr\n"
                                   "-----END CERTIFICATE-----\n";

const BCHAR *fabric_org2_tlsCert = "-----BEGIN CERTIFICATE-----\n"
                                   "MIICVjCCAf2gAwIBAgIQEnPt7b92LrvM7Y+lYDFvczAKBggqhkjOPQQDAjB2MQsw\n"
                                   "CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
                                   "YW5jaXNjbzEZMBcGA1UEChMQb3JnMi5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\n"
                                   "Y2Eub3JnMi5leGFtcGxlLmNvbTAeFw0yMjA1MDYwMzAxMDBaFw0zMjA1MDMwMzAx\n"
                                   "MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\n"
                                   "Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmcyLmV4YW1wbGUuY29tMR8wHQYD\n"
                                   "VQQDExZ0bHNjYS5vcmcyLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
                                   "AQcDQgAEWu5iFCGmJg+M5Y0c3Tk6AuKA359x1z913vT3dIfsDVZs7CurWtyXnLe7\n"
                                   "RB62rI2xhL4g1Q6cqyj0E6++/l1r/6NtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\n"
                                   "JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
                                   "DgQiBCApfh1dtu1yeW1ES5r3J9M8yUQIdfukNV2OKARmvJCqujAKBggqhkjOPQQD\n"
                                   "AgNHADBEAiBOWqG9hh8SPWLi6OIrHH+kjDXYcJTSgDUkKqP9nexBNwIgNA6A2EPw\n"
                                   "0SKwSU3uC5c+3Tu+20ur3LZ4mjF7cFoXBSk=\n"
                                   "-----END CERTIFICATE-----\n";

const BCHAR *fabric_order_tlsCert = "-----BEGIN CERTIFICATE-----\n"
                                    "MIICQzCCAemgAwIBAgIQaQ52W1nx08Qn/Ib9Wzb4sDAKBggqhkjOPQQDAjBsMQsw\n"
                                    "CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n"
                                    "YW5jaXNjbzEUMBIGA1UEChMLZXhhbXBsZS5jb20xGjAYBgNVBAMTEXRsc2NhLmV4\n"
                                    "YW1wbGUuY29tMB4XDTIyMDUwNjAzMDEwMFoXDTMyMDUwMzAzMDEwMFowbDELMAkG\n"
                                    "A1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBGcmFu\n"
                                    "Y2lzY28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRowGAYDVQQDExF0bHNjYS5leGFt\n"
                                    "cGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABBfjiM3cLsoDt3tc5Nds\n"
                                    "cRPYccnZb3hj+J6e0/4C0ZAeZX6YjXh5gRmVvg36pITV7L1mlAy+9MusCts1UhiS\n"
                                    "MPmjbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNVHSUEFjAUBggrBgEFBQcDAgYIKwYB\n"
                                    "BQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNVHQ4EIgQgsVjTxGcAStIyI7H59m+t\n"
                                    "Mgz3A0LvBl8+zw+Oa38ImA4wCgYIKoZIzj0EAwIDSAAwRQIhAN1+GWbcyN3i45rm\n"
                                    "9zr4ss8pD2Xgwe8t5/DuGGC/7KfaAiBwaH9saVyHOAvPMSNCRUsLSNkOgnOO7G0y\n"
                                    "Qy2dzpelIg==\n"
                                    "-----END CERTIFICATE-----\n";

const BCHAR *fabric_demo_order1_url = "127.0.0.0:7050";
const BCHAR *fabric_demo_order1_hostName = "orderer.example.com";

const BCHAR *fabric_demo_endorser_peer0Org1_url = "127.0.0.0:7051";
const BCHAR *fabric_demo_endorser_peer0Org1_hostName = "peer0.org1.example.com";
const BCHAR *fabric_demo_endorser_peer1Org1_url = "127.0.0.0:8051";
const BCHAR *fabric_demo_endorser_peer1Org1_hostName = "peer1.org1.example.com";

const BCHAR *fabric_demo_endorser_peer0Org2_url = "127.0.0.0:9051";
const BCHAR *fabric_demo_endorser_peer0Org2_hostName = "peer0.org2.example.com";
const BCHAR *fabric_demo_endorser_peer1Org2_url = "127.0.0.0:10051";
const BCHAR *fabric_demo_endorser_peer1Org2_hostName = "peer1.org2.example.com";

const BCHAR *hw_bcs_client_demokey = "-----BEGIN PRIVATE KEY-----\n"
                                     "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQg9d6xlIhBpXL1jyGg\n"
                                     "1cIqhPXvAzf/U/y1qfJAPnC1uiKhRANCAAQEiZePJltVa2rl8ew2lFkE0qMVhYm5\n"
                                     "EzaEs67tFKPDe421v2datMggAo0gJop9NrRO1P2NN5WkvaSfCG8kwyMK\n"
                                     "-----END PRIVATE KEY-----\n";

const BCHAR *hw_bcs_client_democert = "-----BEGIN CERTIFICATE-----\n"
                                      "MIIClDCCAjqgAwIBAgIQUoHlTcUu5HjLbv2HSUilBzAKBggqhkjOPQQDAjCBhjEO\n"
                                      "MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
                                      "bnpoZW4xDTALBgNVBAoTBG9yZzExDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
                                      "Y2Eub3JnMS5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
                                      "NTE0OFoXDTMxMDgxOTA0NTE0OFowgYoxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
                                      "EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcxMQ8w\n"
                                      "DQYDVQQLEwZIdWF3ZWkxMTAvBgNVBAMTKHVzZXItMS5vcmcxLmJjcy1lcG1tbGku\n"
                                      "c3ZjLmNsdXN0ZXIubG9jYWwwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQEiZeP\n"
                                      "JltVa2rl8ew2lFkE0qMVhYm5EzaEs67tFKPDe421v2datMggAo0gJop9NrRO1P2N\n"
                                      "N5WkvaSfCG8kwyMKo4GDMIGAMA4GA1UdDwEB/wQEAwIFoDAMBgNVHRMBAf8EAjAA\n"
                                      "MCsGA1UdIwQkMCKAIK1MsN0yPniDh8jtrSxAvxwR2XpL8dCm0lTvqN4tNlWVMDMG\n"
                                      "A1UdEQQsMCqCKHVzZXItMS5vcmcxLmJjcy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9j\n"
                                      "YWwwCgYIKoZIzj0EAwIDSAAwRQIhAPECJCuqWtLi9cfnxG31IlBGfD4OKII8svV3\n"
                                      "URYn+CQKAiBzKCZk1g+5GH/DvqqJhspp6zB1GsoUXwN/o641s3l4bg==\n"
                                      "-----END CERTIFICATE-----\n";

const BCHAR *hw_bcs_org1_tlsCert = "-----BEGIN CERTIFICATE-----\n"
                                   "MIICrDCCAlKgAwIBAgIQdvsQdvTNG+rVbXHhnOS9mzAKBggqhkjOPQQDAjCBhjEO\n"
                                   "MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
                                   "bnpoZW4xDTALBgNVBAoTBG9yZzExDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
                                   "Y2Eub3JnMS5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
                                   "NTE0OFoXDTMxMDgxOTA0NTE0OFowgYYxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
                                   "EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcxMQ8w\n"
                                   "DQYDVQQLEwZIdWF3ZWkxLTArBgNVBAMTJGNhLm9yZzEuYmNzLWVwbW1saS5zdmMu\n"
                                   "Y2x1c3Rlci5sb2NhbDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABID+pcLWFgiw\n"
                                   "4nlEQNma0QFtlLrDV0YXfyKB1EwmyGynOosbf1mz9qzbzCjtUyZhDJnOhTSbq8uf\n"
                                   "D7OOf9m5JeijgZ8wgZwwDgYDVR0PAQH/BAQDAgGmMB0GA1UdJQQWMBQGCCsGAQUF\n"
                                   "BwMBBggrBgEFBQcDAjAPBgNVHRMBAf8EBTADAQH/MCkGA1UdDgQiBCC+pd8v/ch2\n"
                                   "igg6x8bgJHiBMFB/ABiul6O58eNruCn+IzAvBgNVHREEKDAmgiRjYS5vcmcxLmJj\n"
                                   "cy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9jYWwwCgYIKoZIzj0EAwIDSAAwRQIgIR4F\n"
                                   "YQDnjxHgoOha5N1Ec1aee+8R+n+un6Vu/q5G7RcCIQD0h5Ylb4ytyFxo21QhtFJR\n"
                                   "Jq7sq/hJ+yziR0ZrVnKXeA==\n"
                                   // "MIIClDCCAjqgAwIBAgIQUfKiHXReppD/PuCucebvDTAKBggqhkjOPQQDAjCBhjEO\n"
                                   // "MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
                                   // "bnpoZW4xDTALBgNVBAoTBG9yZzExDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
                                   // "Y2Eub3JnMS5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
                                   // "NTE0OFoXDTMxMDgxOTA0NTE0OFowgYoxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
                                   // "EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcxMQ8w\n"
                                   // "DQYDVQQLEwZIdWF3ZWkxMTAvBgNVBAMTKHVzZXItMS5vcmcxLmJjcy1lcG1tbGku\n"
                                   // "c3ZjLmNsdXN0ZXIubG9jYWwwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQ8JbFm\n"
                                   // "u20E7yHZWMkadZJ4QUBL96kWE97bmGukJs0nc6DS34OXmZHIiDetQkNkCX5bcsOw\n"
                                   // "m0hVXQnrkGQdFEIFo4GDMIGAMA4GA1UdDwEB/wQEAwIFoDAMBgNVHRMBAf8EAjAA\n"
                                   // "MCsGA1UdIwQkMCKAIL6l3y/9yHaKCDrHxuAkeIEwUH8AGK6Xo7nx42u4Kf4jMDMG\n"
                                   // "A1UdEQQsMCqCKHVzZXItMS5vcmcxLmJjcy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9j\n"
                                   // "YWwwCgYIKoZIzj0EAwIDSAAwRQIhAMGaCGD1uKTW4B8PNHoMfTrBgJmjmSbOMV26\n"
                                   // "JDK8ZAySAiBMMQGhCdMJlVCOLphkHYiExVQJa1nMaos0xzAOX9UBUQ==\n"

                                   "-----END CERTIFICATE-----\n";

const BCHAR *hw_bcs_org2_tlsCert = "-----BEGIN CERTIFICATE-----\n"
                                   "MIICqzCCAlKgAwIBAgIQQgfD2Vp1Gr+aAo4w73iXbzAKBggqhkjOPQQDAjCBhjEO\n"
                                   "MAwGA1UEBhMFQ2hpbmExEjAQBgNVBAgTCUd1YW5nZG9uZzERMA8GA1UEBxMIU2hl\n"
                                   "bnpoZW4xDTALBgNVBAoTBG9yZzIxDzANBgNVBAsTBkh1YXdlaTEtMCsGA1UEAxMk\n"
                                   "Y2Eub3JnMi5iY3MtZXBtbWxpLnN2Yy5jbHVzdGVyLmxvY2FsMB4XDTIxMDgyMTA0\n"
                                   "NTE0OFoXDTMxMDgxOTA0NTE0OFowgYYxDjAMBgNVBAYTBUNoaW5hMRIwEAYDVQQI\n"
                                   "EwlHdWFuZ2RvbmcxETAPBgNVBAcTCFNoZW56aGVuMQ0wCwYDVQQKEwRvcmcyMQ8w\n"
                                   "DQYDVQQLEwZIdWF3ZWkxLTArBgNVBAMTJGNhLm9yZzIuYmNzLWVwbW1saS5zdmMu\n"
                                   "Y2x1c3Rlci5sb2NhbDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABI1P9XWwX1Ub\n"
                                   "c3kGY6OOfrh/jGVEM2Aci8+tZozrpxASfj3O5/YNPFjXJAbCDHs+/cqFBNTXxL0Y\n"
                                   "zQ8G2mEzW/2jgZ8wgZwwDgYDVR0PAQH/BAQDAgGmMB0GA1UdJQQWMBQGCCsGAQUF\n"
                                   "BwMBBggrBgEFBQcDAjAPBgNVHRMBAf8EBTADAQH/MCkGA1UdDgQiBCAWIw2jwEOU\n"
                                   "TqzkfhdBOMtJmHphhAD0FLxlSCPeP8UGYzAvBgNVHREEKDAmgiRjYS5vcmcyLmJj\n"
                                   "cy1lcG1tbGkuc3ZjLmNsdXN0ZXIubG9jYWwwCgYIKoZIzj0EAwIDRwAwRAIgKSMJ\n"
                                   "YDSTPdX0BsiLKu0f0IFauU1E/pgi3jMU3SuhPxgCIFUuVfU/pfTRxK6UGFYH5uxc\n"
                                   "AtrZMPym0MX872awTHPL\n"
                                   "-----END CERTIFICATE-----\n";

const BCHAR *hw_bcs_demo_endorser_peer0Org1_url = "49.4.115.203:30616";
const BCHAR *hw_bcs_demo_endorser_peer0Org1_hostName = "node-0.org1.bcs-epmmli.svc.cluster.local";

const BCHAR *hw_bcs_demo_endorser_peer0Org2_url = "49.4.115.203:30606";
const BCHAR *hw_bcs_demo_endorser_peer0Org2_hostName = "node-0.org2.bcs-epmmli.svc.cluster.local";

const BCHAR *hw_bcs_demo_order_url = "49.4.115.203:30616";
const BCHAR *hw_bcs_demo_order_hostName = "node-0.org1.bcs-epmmli.svc.cluster.local";

const BCHAR *chainmaker_client_sign_prikey = "-----BEGIN EC PRIVATE KEY-----\n"
                                             "MHcCAQEEIOyaQAt/ksxq8VOlkJqvFyne60m8IjaRMCaDgmVACOF3oAoGCCqGSM49\n"
                                             "AwEHoUQDQgAEvhmUZnXAdpVPAmXTIcP3XVAXxUJtmL20xatRqkW7Kyg8Ghlg4jVW\n"
                                             "CYCeeChl4OBno9aCLc5BvFk4tmKDdOp96g==\n"
                                             "-----END EC PRIVATE KEY-----\n";

const BCHAR *native_demoKey = "0xec9a400b7f92cc6af153a5909aaf1729deeb49bc22369130268382654008e177";

const BCHAR *chainmaker_clinet_tls_prikey = "-----BEGIN EC PRIVATE KEY-----\n"
                                            "MHcCAQEEIOyaQAt/ksxq8VOlkJqvFyne60m8IjaRMCaDgmVACOF3oAoGCCqGSM49\n"
                                            "AwEHoUQDQgAEvhmUZnXAdpVPAmXTIcP3XVAXxUJtmL20xatRqkW7Kyg8Ghlg4jVW\n"
                                            "CYCeeChl4OBno9aCLc5BvFk4tmKDdOp96g==\n"
                                            "-----END EC PRIVATE KEY-----\n";

const BCHAR *chainmaker_client_sign_cert = "-----BEGIN CERTIFICATE-----\n"
                                           "MIICdzCCAh6gAwIBAgIDC9o/MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
                                           "MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
                                           "b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
                                           "ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIyMDExMTA4NDk0OFoXDTI3\n"
                                           "MDExMDA4NDk0OFowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
                                           "DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
                                           "MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
                                           "LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEvhmUZnXA\n"
                                           "dpVPAmXTIcP3XVAXxUJtmL20xatRqkW7Kyg8Ghlg4jVWCYCeeChl4OBno9aCLc5B\n"
                                           "vFk4tmKDdOp96qNqMGgwDgYDVR0PAQH/BAQDAgbAMCkGA1UdDgQiBCDfUK7TyHf+\n"
                                           "FJl27KT6uoRgcDOmNgwmc5G0A2dhn5fQfTArBgNVHSMEJDAigCAMxN5G+OtJXgmo\n"
                                           "0MEU1OcJTBuDkfakZV07kDTZI9TcpjAKBggqhkjOPQQDAgNHADBEAiBEmn2YPZ0r\n"
                                           "zzPwilsgtOcoPc6PkOawUnWIU6d7iehfIQIgaqDwoWO6nwT0q0Tasr2+suqidv9V\n"
                                           "WOhSqMzOl8zMQ0c=\n"
                                           "-----END CERTIFICATE-----\n";

const BCHAR *chainmaker_client_tls_cert = "-----BEGIN CERTIFICATE-----\n"
                                          "MIICnzCCAkSgAwIBAgIDBXs2MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
                                          "MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
                                          "b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
                                          "ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIyMDExMTA4NDk0OFoXDTMy\n"
                                          "MDEwOTA4NDk0OFowgYoxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
                                          "DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
                                          "MRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMTGWNhLnd4LW9yZzEuY2hhaW5t\n"
                                          "YWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASb/f+wD0tJVZ78bXBF\n"
                                          "LU/tar8wOoJi8+5qFwifEUwe4WdTBZJeqcqxn2/pcErzXnzGfcHMY609CU8sW2Bb\n"
                                          "zOyTo4GWMIGTMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
                                          "DgQiBCAMxN5G+OtJXgmo0MEU1OcJTBuDkfakZV07kDTZI9TcpjBFBgNVHREEPjA8\n"
                                          "gg5jaGFpbm1ha2VyLm9yZ4IJbG9jYWxob3N0ghljYS53eC1vcmcxLmNoYWlubWFr\n"
                                          "ZXIub3JnhwR/AAABMAoGCCqGSM49BAMCA0kAMEYCIQCoXy5qAIGrfnxRop9pgy9A\n"
                                          "XQdXmu1mlryNGVmfisnm2gIhAMjQP7CTwoJDeu8bs36i+7ha4bIHct+pmE5BpjYh\n"
                                          "ln6O\n"
                                          "-----END CERTIFICATE-----\n";

BCHAR *chainmaker_node_url = "127.0.0.0:12301";
BCHAR *chainmaker_host_name = "chainmaker.org";
BCHAR *chainmaker_chain_id = "chain1";
BCHAR *chainmaker_org_id = "wx-org1.chainmaker.org";

const BUINT8 gBinChainId[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
const BUINT32 version = 2;

/**
****************************************************************************************
* @brief:
*  This function get the configuration to create eth network;
* @param[in] *config
*  output of the configuration
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT getEthNetworkConfig(BoatEthNetworkConfig *config)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check param*/
    if (NULL == config)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if ((strlen(demoUrl) + 1) > sizeof(config->node_url_str))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    config->chain_id = chainID;
    config->eip155_compatibility = BOAT_FALSE;
    strcpy(config->node_url_str, demoUrl);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the configuration to create eth network;
* @param[in] *config
*  output of the configuration
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT getPlatONNetworkConfig(BoatPlatONNetworkConfig *config)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check param*/
    if (NULL == config)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if ((strlen(demoUrl) + 1) > sizeof(config->node_url_str))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    config->chain_id = chainID;
    config->eip155_compatibility = BOAT_FALSE;
    strcpy(config->node_url_str, demoUrl);
    return result;
}
/**
****************************************************************************************
*  @brief:
*  This function get the configuration to create eth network;
* @param[in] *config
*  output of the configuration
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT getPlatoneNetworkConfig(BoatPlatoneNetworkConfig *config)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check param*/
    if (NULL == config)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if ((strlen(demoUrl) + 1) > sizeof(config->node_url_str))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    config->chain_id = chainID;
    config->eip155_compatibility = BOAT_FALSE;
    strcpy(config->node_url_str, demoUrl);
    return result;
}
/**
****************************************************************************************
* @brief:
*  This function get the configuration to create fiscobcos network;
* @param[in/out] *config
* @param[in] networkIndex
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
static BOAT_RESULT getFiscobcosNetworkConfig(BoatFiscobcosNetworkConfig *config)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check param*/
    if (NULL == config)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if ((strlen(demoUrl) + 1) > sizeof(config->node_url_str))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    config->chain_id = chainID;
    config->eip155_compatibility = BOAT_FALSE;
    strcpy(config->node_url_str, demoUrl);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of fabric
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT fabric_getNetworkCfg(BoatHlfabricNetworkConfig *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set cert context
    networkConfig->accountCertContent.length = strlen(fabric_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, fabric_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tlsCert);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_hostName) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_hostName) + 1);
    // memset(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, 0, strlen(fabric_demo_endorser_peer0Org1_url) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, fabric_demo_endorser_peer0Org1_hostName);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName, fabric_demo_endorser_peer1Org1_hostName);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(fabric_org2_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, fabric_org2_tlsCert);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_hostName) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_hostName) + 1);
    // memset(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, 0, strlen(fabric_demo_endorser_peer0Org2_url) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org2_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, fabric_demo_endorser_peer0Org2_hostName);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org2_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org2_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName, fabric_demo_endorser_peer1Org2_hostName);

    // set orderer info
    networkConfig->nodesCfg.orderCfg.endorserNumber = 1;
    networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(fabric_order_tlsCert);
    strcpy(networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.content, fabric_order_tlsCert);
    networkConfig->nodesCfg.orderCfg.endorser = BoatMalloc(networkConfig->nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(fabric_demo_order1_hostName) + 1);
    networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_order1_url) + 1);
    memset(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, 0, strlen(fabric_demo_order1_url) + 1);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, fabric_demo_order1_url);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].hostName, fabric_demo_order1_hostName);

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
    networkConfig->accountClientTlsPrikey.value_len = strlen(fabric_client_tls_prikey);
    strcpy((BCHAR *)networkConfig->accountClientTlsPrikey.value, fabric_client_tls_prikey);
    networkConfig->accountClientTlsCert.length = strlen(fabric_client_tls_cert);
    strcpy((BCHAR *)networkConfig->accountClientTlsCert.content, fabric_client_tls_cert);
#endif
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of hwbcs
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT hwbcs_getNetworkCfg(BoatHwbcsNetworkConfig *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set cert context
    networkConfig->accountCertContent.length = strlen(hw_bcs_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, hw_bcs_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHwbcsNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHwbcsNodeGroupCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org1_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org1_hostName) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, hw_bcs_demo_endorser_peer0Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, hw_bcs_demo_endorser_peer0Org1_hostName);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, hw_bcs_org1_tlsCert);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org2_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_endorser_peer0Org2_hostName) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, hw_bcs_demo_endorser_peer0Org2_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, hw_bcs_demo_endorser_peer0Org2_hostName);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(hw_bcs_org2_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, hw_bcs_org2_tlsCert);

    networkConfig->nodesCfg.orderCfg.endorserNumber = 1;
    networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(hw_bcs_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.content, hw_bcs_org1_tlsCert);
    networkConfig->nodesCfg.orderCfg.endorser = BoatMalloc(networkConfig->nodesCfg.orderCfg.endorserNumber * sizeof(BoatHwbcsNodeInfoCfg));
    networkConfig->nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(hw_bcs_demo_order_hostName) + 1);
    networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(hw_bcs_demo_order_url) + 1);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, hw_bcs_demo_order_url);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].hostName, hw_bcs_demo_order_hostName);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the configuration to create quorum network;
* @param[in/out] *config
*   output of the configuration
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT getQuorumNetworkConfig(BoatQuorumNetworkConfig *config)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check param*/
    if (NULL == config)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if ((strlen(demoUrl) + 1) > sizeof(config->node_url_str))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    config->chain_id = chainID;
    config->eip155_compatibility = BOAT_FALSE;
    strcpy(config->node_url_str, demoUrl);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the configuration to create venachain network;
* @param[in/out] *config
*   output of the configuration
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT getVenachainNetworkConfig(BoatVenachainNetworkConfig *config)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check param*/
    if (NULL == config)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if ((strlen(demoUrl) + 1) > sizeof(config->node_url_str))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    config->chain_id = chainID;
    config->eip155_compatibility = BOAT_FALSE;
    strcpy(config->node_url_str, demoUrl);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the configuration to create chainmaker network;
* @param[in] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT getChainmakerNetworkConfig(BoatChainmakerNetworkData *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set user cert context
    networkConfig->client_sign_cert_content.length = strlen(chainmaker_client_sign_cert) + 1;
    if (networkConfig->client_sign_cert_content.length > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    memcpy(networkConfig->client_sign_cert_content.content, chainmaker_client_sign_cert, networkConfig->client_sign_cert_content.length);

    // set url and name
    if ((strlen(chainmaker_node_url) >= BAOT_CHAINMAKER_NODE_STR_LEN) ||
        (strlen(chainmaker_host_name) >= BAOT_CHAINMAKER_NODE_STR_LEN) ||
        (strlen(chainmaker_chain_id) >= BAOT_CHAINMAKER_NODE_STR_LEN) ||
        (strlen(chainmaker_org_id) >= BAOT_CHAINMAKER_NODE_STR_LEN))
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // strncpy(networkConfig->node_url, chainmaker_node_url, strlen(chainmaker_node_url));
    // strncpy(networkConfig->host_name, chainmaker_host_name, strlen(chainmaker_host_name));
    // strncpy(networkConfig->chain_id, chainmaker_chain_id, strlen(chainmaker_chain_id));
    // strncpy(networkConfig->org_id, chainmaker_org_id, strlen(chainmaker_org_id));
    strcpy(networkConfig->node_url, chainmaker_node_url);
    strcpy(networkConfig->host_name, chainmaker_host_name);
    strcpy(networkConfig->chain_id, chainmaker_chain_id);
    strcpy(networkConfig->org_id, chainmaker_org_id);

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    // set tls cert context
    networkConfig->client_tls_cert_content.length = strlen(chainmaker_client_tls_cert) + 1;
    if (networkConfig->client_tls_cert_content.length > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    memcpy(networkConfig->client_tls_cert_content.content, chainmaker_client_tls_cert, networkConfig->client_tls_cert_content.length);
#endif

#if (BOAT_CHAINMAKR_TLS_IDENTIFY_CLIENT == 1)
    networkConfig->client_tls_privkey_content.value_len = strlen(chainmaker_clinet_tls_prikey) + 1;
    if (networkConfig->client_tls_privkey_content.value_len > BOAT_CHAINMAKER_PRIKEY_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    memcpy(networkConfig->client_tls_privkey_content.value, chainmaker_clinet_tls_prikey, networkConfig->client_tls_privkey_content.value_len);
#endif
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the configuration to create cita network;
* @param[in/out] *config
*  output of the configuration
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT getCitaNetworkConfig(BoatCitaNetworkConfig *config)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check param*/
    if (NULL == config)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if ((strlen(demoUrl) + 1) > sizeof(config->node_url_buf))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    memcpy(&config->chain_id_buf, gBinChainId, sizeof(gBinChainId));
    config->version = version;
    strcpy(config->node_url_buf, demoUrl);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime Eth network success;
*  get network by index with all the blockchains supported;
*  Only the interface with Eth return success, the other's return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0001CreateOneTimeNetworkSuccess_ETH)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatEthNetworkConfig network_config;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;
    /* get configuration */
    rtnVal = getEthNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATEthNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATEth_GetNetworkByIndex(&Ethnetworkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get platon network by index */
    rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no PlatON network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, 0);
    /* check the result of getting network data, There has no Platone network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fiscobcos network by index */
    rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, 0);
    /* check the result of getting network data, There has no fiscobcos network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fabric network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, 0);
    /* check the result of getting network data, There has no fabric network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get hwbcs network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, 0);
    /* check the result of getting network data, There has no hwbcs network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get quorum network by index */
    rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, 0);
    /* check the result of getting network data, There has no quorum network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get venachain network by index */
    rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, 0);
    /* check the result of getting network data, There has no venachain network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get chainmaker network by index */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, 0);
    /* check the result of getting network data, There has no chainmaker network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get cita network by index */
    rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, 0);
    /* check the result of getting network data, There has no cita network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* delete the onetime networl */
    BoATEthNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime PlatON network success;
*  get network by index with all the blockchains supported;
*  Only the interface with PlatON return success, the other's return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0002CreateOneTimeNetworkSuccess_platON)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatPlatONNetworkConfig network_config;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;
    /* get configuration */
    rtnVal = getPlatONNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATPlatONNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get platon network by index */
    rtnVal = BoATEth_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no Eth network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, 0);
    /* check the result of getting network data, There has no Platone network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fiscobcos network by index */
    rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, 0);
    /* check the result of getting network data, There has no fiscobcos network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fabric network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, 0);
    /* check the result of getting network data, There has no fabric network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get hwbcs network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, 0);
    /* check the result of getting network data, There has no hwbcs network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get quorum network by index */
    rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, 0);
    /* check the result of getting network data, There has no quorum network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get venachain network by index */
    rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, 0);
    /* check the result of getting network data, There has no venachain network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get chainmaker network by index */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, 0);
    /* check the result of getting network data, There has no chainmaker network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get cita network by index */
    rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, 0);
    /* check the result of getting network data, There has no cita network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* delete the onetime networl */
    BoATEthNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime platone network success;
*  get network by index with all the blockchains supported;
*  Only the interface with platone return success, the other's return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0003CreateOneTimeNetworkSuccess_platone)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatPlatoneNetworkConfig network_config;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;
    /* get configuration */
    rtnVal = getPlatoneNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get platon network by index */
    rtnVal = BoATEth_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no Eth network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no PlatON network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fiscobcos network by index */
    rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, 0);
    /* check the result of getting network data, There has no fiscobcos network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fabric network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, 0);
    /* check the result of getting network data, There has no fabric network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get hwbcs network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, 0);
    /* check the result of getting network data, There has no hwbcs network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get quorum network by index */
    rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, 0);
    /* check the result of getting network data, There has no quorum network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get venachain network by index */
    rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, 0);
    /* check the result of getting network data, There has no venachain network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get chainmaker network by index */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, 0);
    /* check the result of getting network data, There has no chainmaker network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get cita network by index */
    rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, 0);
    /* check the result of getting network data, There has no cita network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* delete the onetime networl */
    BoATEthNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime fiscobcos network success;
*  get network by index with all the blockchains supported;
*  Only the interface with fiscobcos return success, the other's return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0004CreateOneTimeNetworkSuccess_fiscobcos)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatFiscobcosNetworkConfig network_config;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;
    /* get configuration */
    rtnVal = getFiscobcosNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATFiscobcosNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get platon network by index */
    rtnVal = BoATEth_GetNetworkByIndex(&Ethnetworkdata, 0);
    /* check the result of getting network data, There has no Eth network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no PlatON network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, 0);
    /* check the result of getting network data, There has no platone network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fabric network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, 0);
    /* check the result of getting network data, There has no fabric network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get hwbcs network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, 0);
    /* check the result of getting network data, There has no hwbcs network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get quorum network by index */
    rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, 0);
    /* check the result of getting network data, There has no quorum network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get venachain network by index */
    rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, 0);
    /* check the result of getting network data, There has no venachain network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get chainmaker network by index */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, 0);
    /* check the result of getting network data, There has no chainmaker network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get cita network by index */
    rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, 0);
    /* check the result of getting network data, There has no cita network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* delete the onetime networl */
    BoATEthNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime fabric network success;
*  get network by index with all the blockchains supported;
*  Only the interface with fabric return success, the other's return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0005CreateOneTimeNetworkSuccess_fabric)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatHlfabricNetworkConfig network_config;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;
    /* get configuration */
    rtnVal = fabric_getNetworkCfg(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatHlfabricNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* free configuration struct */
    fabricWalletConfigFree(network_config);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* free network data*/
    BoATHlfabric_FreeNetworkData(Fabricnetworkdata);
    /* get platon network by index */
    rtnVal = BoATEth_GetNetworkByIndex(&Ethnetworkdata, 0);
    /* check the result of getting network data, There has no Eth network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no PlatON network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, 0);
    /* check the result of getting network data, There has no platone network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fiscobcos network by index */
    rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, 0);
    /* check the result of getting network data, There has no ficobcos network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get hwbcs network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, 0);
    /* check the result of getting network data, There has no hwbcs network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get quorum network by index */
    rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, 0);
    /* check the result of getting network data, There has no quorum network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get venachain network by index */
    rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, 0);
    /* check the result of getting network data, There has no venachain network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get chainmaker network by index */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, 0);
    /* check the result of getting network data, There has no chainmaker network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get cita network by index */
    rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, 0);
    /* check the result of getting network data, There has no cita network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* delete the onetime networl */
    BoATEthNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime hwbcs network success;
*  get network by index with all the blockchains supported;
*  Only the interface with hwbcs return success, the other's return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0006CreateOneTimeNetworkSuccess_hwbcs)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatHwbcsNetworkConfig network_config;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;
    /* get configuration */
    rtnVal = hwbcs_getNetworkCfg(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATHwbcsNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* free configuration struct */
    hwbcsWalletConfigFree(network_config);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, 0);
    /* free network data*/
    BoATHwbcs_FreeNetworkData(Hwbcsnetworkdata);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get platon network by index */
    rtnVal = BoATEth_GetNetworkByIndex(&Ethnetworkdata, 0);
    /* check the result of getting network data, There has no Eth network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no PlatON network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, 0);
    /* check the result of getting network data, There has no platone network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fiscobcos network by index */
    rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, 0);
    /* check the result of getting network data, There has no ficobcos network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fabric network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, 0);
    /* check the result of getting network data, There has no fabric network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get quorum network by index */
    rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, 0);
    /* check the result of getting network data, There has no quorum network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get venachain network by index */
    rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, 0);
    /* check the result of getting network data, There has no venachain network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get chainmaker network by index */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, 0);
    /* check the result of getting network data, There has no chainmaker network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get cita network by index */
    rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, 0);
    /* check the result of getting network data, There has no cita network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* delete the onetime networl */
    BoATEthNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime quorum network success;
*  get network by index with all the blockchains supported;
*  Only the interface with quorum return success, the other's return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0007CreateOneTimeNetworkSuccess_quorum)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatQuorumNetworkConfig network_config;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;
    /* get configuration */
    rtnVal = getQuorumNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatQuorumNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get platon network by index */
    rtnVal = BoATEth_GetNetworkByIndex(&Ethnetworkdata, 0);
    /* check the result of getting network data, There has no Eth network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no PlatON network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, 0);
    /* check the result of getting network data, There has no platone network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fiscobcos network by index */
    rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, 0);
    /* check the result of getting network data, There has no ficobcos network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fabric network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, 0);
    /* check the result of getting network data, There has no fabric network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get hwbcs network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, 0);
    /* check the result of getting network data, There has no hwbcs network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get venachain network by index */
    rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, 0);
    /* check the result of getting network data, There has no venachain network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get chainmaker network by index */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, 0);
    /* check the result of getting network data, There has no chainmaker network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get cita network by index */
    rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, 0);
    /* check the result of getting network data, There has no cita network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* delete the onetime networl */
    BoATEthNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime venachain network success;
*  get network by index with all the blockchains supported;
*  Only the interface with venachain return success, the other's return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0008CreateOneTimeNetworkSuccess_venachain)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatVenachainNetworkConfig network_config;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;
    /* get configuration */
    rtnVal = getVenachainNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatVenachainNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get platon network by index */
    rtnVal = BoATEth_GetNetworkByIndex(&Ethnetworkdata, 0);
    /* check the result of getting network data, There has no Eth network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no PlatON network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, 0);
    /* check the result of getting network data, There has no platone network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fiscobcos network by index */
    rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, 0);
    /* check the result of getting network data, There has no ficobcos network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fabric network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, 0);
    /* check the result of getting network data, There has no fabric network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get hwbcs network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, 0);
    /* check the result of getting network data, There has no hwbcs network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get quorum network by index */
    rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, 0);
    /* check the result of getting network data, There has no quorum network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get chainmaker network by index */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, 0);
    /* check the result of getting network data, There has no chainmaker network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get cita network by index */
    rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, 0);
    /* check the result of getting network data, There has no cita network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* delete the onetime networl */
    BoATEthNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime cita network success;
*  get network by index with all the blockchains supported;
*  Only the interface with cita return success, the other's return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0009CreateOneTimeNetworkSuccess_cita)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatCitaNetworkConfig network_config;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;
    /* get configuration */
    rtnVal = getCitaNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATCitaNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get platon network by index */
    rtnVal = BoATEth_GetNetworkByIndex(&Ethnetworkdata, 0);
    /* check the result of getting network data, There has no Eth network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no PlatON network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, 0);
    /* check the result of getting network data, There has no platone network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fiscobcos network by index */
    rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, 0);
    /* check the result of getting network data, There has no ficobcos network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fabric network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, 0);
    /* check the result of getting network data, There has no fabric network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get hwbcs network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, 0);
    /* check the result of getting network data, There has no hwbcs network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get quorum network by index */
    rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, 0);
    /* check the result of getting network data, There has no quorum network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get chainmaker network by index */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, 0);
    /* check the result of getting network data, There has no chainmaker network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get venachain network by index */
    rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, 0);
    /* check the result of getting network data, There has no venachain network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* delete the onetime networl */
    BoATEthNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime chainmaker network success;
*  get network by index with all the blockchains supported;
*  Only the interface with chainmaker return success, the other's return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0010CreateOneTimeNetworkSuccess_chainmaker)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatChainmakerNetworkData network_config;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;
    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoATChainmakerNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get platon network by index */
    rtnVal = BoATEth_GetNetworkByIndex(&Ethnetworkdata, 0);
    /* check the result of getting network data, There has no Eth network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, 0);
    /* check the result of getting network data, There has no PlatON network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get platone network by index */
    rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, 0);
    /* check the result of getting network data, There has no platone network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fiscobcos network by index */
    rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, 0);
    /* check the result of getting network data, There has no ficobcos network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get fabric network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, 0);
    /* check the result of getting network data, There has no fabric network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get hwbcs network by index */
    rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, 0);
    /* check the result of getting network data, There has no hwbcs network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get quorum network by index */
    rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, 0);
    /* check the result of getting network data, There has no quorum network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get cita network by index */
    rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, 0);
    /* check the result of getting network data, There has no cita network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* get venachain network by index */
    rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, 0);
    /* check the result of getting network data, There has no venachain network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    /* delete the onetime networl */
    BoATEthNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create  ETH/PlatON/Platone/fiscobcos/fabric persist network success;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0011Create5PersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatEthNetworkConfig Eth_network_config;
    BoatPlatONNetworkConfig PlatON_network_config;
    BoatPlatoneNetworkConfig Platone_network_config;
    BoatFiscobcosNetworkConfig Fiscobcos_network_config;
    BoatHlfabricNetworkConfig Fabric_network_config;

    /* get configuration */
    rtnVal = getEthNetworkConfig(&Eth_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATEthNetworkCreate(&Eth_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        This is the first persist network, index must be 1 */
    ck_assert_int_eq(rtnVal, 1);

    /* get configuration */
    rtnVal = getPlatONNetworkConfig(&PlatON_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATPlatONNetworkCreate(&PlatON_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        This is the second persist network, index must be 2 */
    ck_assert_int_eq(rtnVal, 2);

    /* get configuration */
    rtnVal = getPlatoneNetworkConfig(&Platone_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATPlatoneNetworkCreate(&Platone_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        This is the third persist network, index must be 3 */
    ck_assert_int_eq(rtnVal, 3);

    /* get configuration */
    rtnVal = getFiscobcosNetworkConfig(&Fiscobcos_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATFiscobcosNetworkCreate(&Fiscobcos_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        This is the fourth persist network, index must be 4 */
    ck_assert_int_eq(rtnVal, 4);

    /* get configuration */
    rtnVal = fabric_getNetworkCfg(&Fabric_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&Fabric_network_config, BOAT_STORE_TYPE_FLASH);
    fabricWalletConfigFree(Fabric_network_config);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        This is the fifth persist network, index must be 5 */
    ck_assert_int_eq(rtnVal, 5);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create 6th persist network fail;
*  already created BOAT_MAX_NETWORK_NUM persist networks, could not create more networks
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0012Create6thPersistNetworkFail)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatHwbcsNetworkConfig Hwbcs_network_config;
    BoatChainmakerNetworkData Chainmaker_network_config;
    BoatVenachainNetworkConfig Venachain_network_config;
    BoatQuorumNetworkConfig Quorum_network_config;
    BoatCitaNetworkConfig Cita_network_config;

    /* get configuration */
    rtnVal = hwbcs_getNetworkCfg(&Hwbcs_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&Hwbcs_network_config, BOAT_STORE_TYPE_FLASH);
    /* free configuration struct */
    hwbcsWalletConfigFree(Hwbcs_network_config);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        Already created BOAT_MAX_NETWORK_NUM persist networks, return BOAT_ERROR_NETWORK_INDEX_EXCEED */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);

    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&Chainmaker_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&Chainmaker_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
       Already created BOAT_MAX_NETWORK_NUM persist networks, return BOAT_ERROR_NETWORK_INDEX_EXCEED */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);

    /* get configuration */
    rtnVal = getVenachainNetworkConfig(&Venachain_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatVenachainNetworkCreate(&Venachain_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        Already created BOAT_MAX_NETWORK_NUM persist networks, return BOAT_ERROR_NETWORK_INDEX_EXCEED */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);

    /* get configuration */
    rtnVal = getQuorumNetworkConfig(&Quorum_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatQuorumNetworkCreate(&Quorum_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
       Already created BOAT_MAX_NETWORK_NUM persist networks, return BOAT_ERROR_NETWORK_INDEX_EXCEED */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);

    /* get configuration */
    rtnVal = getCitaNetworkConfig(&Cita_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATCitaNetworkCreate(&Cita_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
       Already created BOAT_MAX_NETWORK_NUM persist networks, return BOAT_ERROR_NETWORK_INDEX_EXCEED */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network by index;
*  only the index and protocol type are both right, return success;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0013GetNetworkbyIndexSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;

    for (Index = 1; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {
        /* get network data by index , index = 0 */
        rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* get platon network by index */
        rtnVal = BoATEth_GetNetworkByIndex(&Ethnetworkdata, Index);
        if (Index == 1)
        {
            /* persist network which index equal 1 is Eth network */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        else
        {
            /* check the result of getting network data, There has no Eth network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }

        /* get platone network by index */
        rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, Index);
        if (Index == 2)
        {
            /* persist network which index equal 2 is PlatON network */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        else
        {
            /* check the result of getting network data, There has no PlatON network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }

        /* get platone network by index */
        rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, Index);
        if (Index == 3)
        {
            /* persist network which index equal 3 is Platone network */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        else
        {
            /* check the result of getting network data, There has no platone network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }

        /* get fiscobcos network by index */
        rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, Index);
        if (Index == 4)
        {
            /* persist network which index equal 4 is fiscobcos network */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        else
        {
            /* check the result of getting network data, There has no ficobcos network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }

        /* get fabric network by index */
        rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, Index);
        if (Index == 5)
        {
            /* free network data*/
            BoATHlfabric_FreeNetworkData(Fabricnetworkdata);
            /* persist network which index equal 5 is fabric network */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        else
        {
            /* check the result of getting network data, There has no fabric network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }

        /* get hwbcs network by index */
        rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, Index);
        /* check the result of getting network data, There has no hwbcs network */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* get quorum network by index */
        rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, Index);
        /* check the result of getting network data, There has no quorum network */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* get cita network by index */
        rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, Index);
        /* check the result of getting network data, There has no cita network */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* get venachain network by index */
        rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, Index);
        /* check the result of getting network data, There has no venachain network */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete network by index;
*  the index and protocol type don't match, return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0014DeleteNetwork_otherBlockchain)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;

    for (Index = 1; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {
        /* delete network with chainmaker interface, but the protocol type don't macth chainmaker*/
        rtnVal = BoATChainmakerNetworkDelete(Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);

        /* protocol type of network which index equal 2 is platon*/
        if (Index != 2)
        {
            /* delete network with PlatON interface, but the protocol type don't macth PlatON */
            rtnVal = BoATPlatONNetworkDelete(Index);
            /* check the result of getting network data,There has no PlatON network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }
        /* protocol type of network which index equal 1 is eth*/
        if (Index != 1)
        {
            /* delete network with eth interface, but the protocol type don't macth eth  */
            rtnVal = BoATEthNetworkDelete(Index);
            /* check the result of getting network data */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }
        /* protocol type of network which index equal 3 is platone*/
        if (Index != 3)
        {
            /* delete network with platone interface, but the protocol type don't macth platone  */
            rtnVal = BoATPlatoneNetworkDelete(Index);
            /* check the result of getting network data*/
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }
        /* protocol type of network which index equal 4 is fiscobcos*/
        if (Index != 4)
        {
            /* delete network with fiscobcos interface, but the protocol type don't macth fiscobcos  */
            rtnVal = BoATFiscobcosNetworkDelete(Index);
            /* check the result of getting network data */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }
        /* protocol type of network which index equal 5 is fabric*/
        if (Index != 5)
        {
            /* delete network with fabric interface, but the protocol type don't macth fabric  */
            rtnVal = BoATHlfabricNetworkDelete(Index);
            /* check the result of getting network data*/
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }
        /* delete network with hwbcs interface, but the protocol type don't macth hwbcs*/
        rtnVal = BoATHwbcsNetworkDelete(Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* delete network with quorum interface, but the protocol type don't macth quorum*/
        rtnVal = BoATQuorumNetworkDelete(Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* delete network with cita interface, but the protocol type don't macth cita*/
        rtnVal = BoATCitaNetworkDelete(Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* delete network with venachain interface, but the protocol type don't macth venachain*/
        rtnVal = BoATVenachainNetworkDelete(Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete network by index using right interface;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0015DeleteNetwork_Success)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;

    for (Index = 1; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {

        /* protocol type of network which index equal 2 is platon*/
        if (Index == 2)
        {
            /* delete network with PlatON interface, the protocol type macth PlatON */
            rtnVal = BoATPlatONNetworkDelete(Index);
            /* check the result of getting network data */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        /* protocol type of network which index equal 1 is eth*/
        if (Index == 1)
        {
            /* delete network with eth interface, the protocol type macth eth  */
            rtnVal = BoATEthNetworkDelete(Index);
            /* check the result of getting network data */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        /* protocol type of network which index equal 3 is platone*/
        if (Index == 3)
        {
            /* delete network with platone interface, the protocol type macth platone  */
            rtnVal = BoATPlatoneNetworkDelete(Index);
            /* check the result of getting network data*/
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        /* protocol type of network which index equal 4 is fiscobcos*/
        if (Index == 4)
        {
            /* delete network with fiscobcos interface, the protocol type macth fiscobcos  */
            rtnVal = BoATFiscobcosNetworkDelete(Index);
            /* check the result of getting network data */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        /* protocol type of network which index equal 5 is fabric*/
        if (Index == 5)
        {
            /* delete network with fabric interface, the protocol type macth fabric  */
            rtnVal = BoATHlfabricNetworkDelete(Index);
            /* check the result of getting network data*/
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create  Quorum/Venachain/Hwbcs/Chainmaker/Cita persist network success;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0016CreatOtherNetworks_Success)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatQuorumNetworkConfig Quorum_network_config;
    BoatVenachainNetworkConfig Venachain_network_config;
    BoatHwbcsNetworkConfig Hwbcs_network_config;
    BoatChainmakerNetworkData Chainmaker_network_config;
    BoatCitaNetworkConfig Cita_network_config;

    /* get configuration */
    rtnVal = getQuorumNetworkConfig(&Quorum_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatQuorumNetworkCreate(&Quorum_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        This is the first persist network, index must be 1 */
    ck_assert_int_eq(rtnVal, 1);

    /* get configuration */
    rtnVal = getVenachainNetworkConfig(&Venachain_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatVenachainNetworkCreate(&Venachain_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        This is the second persist network, index must be 2 */
    ck_assert_int_eq(rtnVal, 2);

    /* get configuration */
    rtnVal = hwbcs_getNetworkCfg(&Hwbcs_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATHwbcsNetworkCreate(&Hwbcs_network_config, BOAT_STORE_TYPE_FLASH);
    /* free configuration struct */
    hwbcsWalletConfigFree(Hwbcs_network_config);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        This is the third persist network, index must be 3 */
    ck_assert_int_eq(rtnVal, 3);

    /* get configuration */
    rtnVal = getChainmakerNetworkConfig(&Chainmaker_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATChainmakerNetworkCreate(&Chainmaker_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        This is the fourth persist network, index must be 4 */
    ck_assert_int_eq(rtnVal, 4);

    /* get configuration */
    rtnVal = getCitaNetworkConfig(&Cita_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATCitaNetworkCreate(&Cita_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        This is the fifth persist network, index must be 5 */
    ck_assert_int_eq(rtnVal, 5);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create 6th persist network fail;
*  already created BOAT_MAX_NETWORK_NUM persist networks, could not create more networks
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0017CreateAnother6thPersistNetworkFail)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatEthNetworkConfig Eth_network_config;
    BoatPlatONNetworkConfig PlatON_network_config;
    BoatPlatoneNetworkConfig Platone_network_config;
    BoatFiscobcosNetworkConfig Fiscobcos_network_config;
    BoatHlfabricNetworkConfig Hlfabric_network_config;

    /* get configuration */
    rtnVal = getEthNetworkConfig(&Eth_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATEthNetworkCreate(&Eth_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        Already created BOAT_MAX_NETWORK_NUM persist networks, return BOAT_ERROR_NETWORK_INDEX_EXCEED */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);

    /* get configuration */
    rtnVal = getPlatONNetworkConfig(&PlatON_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATPlatONNetworkCreate(&PlatON_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
       Already created BOAT_MAX_NETWORK_NUM persist networks, return BOAT_ERROR_NETWORK_INDEX_EXCEED */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);

    /* get configuration */
    rtnVal = getPlatoneNetworkConfig(&Platone_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATPlatoneNetworkCreate(&Platone_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
        Already created BOAT_MAX_NETWORK_NUM persist networks, return BOAT_ERROR_NETWORK_INDEX_EXCEED */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);

    /* get configuration */
    rtnVal = getFiscobcosNetworkConfig(&Fiscobcos_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoATFiscobcosNetworkCreate(&Fiscobcos_network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
       Already created BOAT_MAX_NETWORK_NUM persist networks, return BOAT_ERROR_NETWORK_INDEX_EXCEED */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);

    /* get configuration */
    rtnVal = fabric_getNetworkCfg(&Hlfabric_network_config);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&Hlfabric_network_config, BOAT_STORE_TYPE_FLASH);
    fabricWalletConfigFree(Hlfabric_network_config);
    /* check index of persist network , index of persist networks is from 1 to BOAT_MAX_NETWORK_NUM;
       Already created BOAT_MAX_NETWORK_NUM persist networks, return BOAT_ERROR_NETWORK_INDEX_EXCEED */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get network by index;
*  only the index and protocol type are both right, return success;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0018GetNetworkbyIndexSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatEthNetworkData Ethnetworkdata;
    BoatPlatONNetworkData PlatONnetworkdata;
    BoatPlatoneNetworkData Platonenetworkdata;
    BoatFiscobcosNetworkData Fiscobcosnetworkdata;
    BoatHlfabricNetworkData Fabricnetworkdata;
    BoatHwbcsNetworkData Hwbcsnetworkdata;
    BoatQuorumNetworkData Quorumnetworkdata;
    BoatVenachainNetworkData Venachainnetworkdata;
    BoatChainmakerNetworkData Chainmakernetworkdata;
    BoatCitaNetworkData Citanetworkdata;

    for (Index = 1; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {

        /* get fiscobcos network by index */
        rtnVal = BoATFiscobcos_GetNetworkByIndex(&Fiscobcosnetworkdata, Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* get quorum network by index */
        rtnVal = BoATQuorum_GetNetworkByIndex(&Quorumnetworkdata, Index);
        if (Index == 1)
        {
            /* persist network which index equal 1 is quorum network */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        else
        {
            /* check the result of getting network data, There has no quorum network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }

        /* get venachain network by index */
        rtnVal = BoATVenachain_GetNetworkByIndex(&Venachainnetworkdata, Index);
        if (Index == 2)
        {
            /* persist network which index equal 2 is venachain network */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        else
        {
            /* check the result of getting network data, There has no venachain network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }

        /* get hwbcs network by index */
        rtnVal = BoATHwbcs_GetNetworkByIndex(&Hwbcsnetworkdata, Index);
        if (Index == 3)
        {
            /* free network data*/
            BoATHwbcs_FreeNetworkData(Hwbcsnetworkdata);
            /* persist network which index equal 3 is hwbcs network */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        else
        {
            /* check the result of getting network data, There has no hwbcs network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }

        /* get network data by index , index = 0 */
        rtnVal = BoATChainmaker_GetNetworkByIndex(&Chainmakernetworkdata, Index);
        if (Index == 4)
        {
            /* persist network which index equal 4 is chainmaker network */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        else
        {
            /* check the result of getting network data, There has no chainmaker network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }

        /* get cita network by index */
        rtnVal = BoATCita_GetNetworkByIndex(&Citanetworkdata, Index);
        if (Index == 5)
        {
            /* persist network which index equal 5 is cita network */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        else
        {
            /* check the result of getting network data, There has no cita network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }

        /* get platone network by index */
        rtnVal = BoATPlatone_GetNetworkByIndex(&Platonenetworkdata, Index);
        /* check the result of getting network data, There has no platone network */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* get platon network by index */
        rtnVal = BoATEth_GetNetworkByIndex(&Ethnetworkdata, Index);
        /* check the result of getting network data, There has no platon network */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* get fabric network by index */
        rtnVal = BoATHlfabric_GetNetworkByIndex(&Fabricnetworkdata, Index);
        /* check the result of getting network data, There has no fabric network */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* get platone network by index */
        rtnVal = BoATPlatON_GetNetworkByIndex(&PlatONnetworkdata, Index);
        /* check the result of getting network data, There has no platone network */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete network by index;
*  the index and protocol type don't match, return error;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0019DeleteNetwork_otherBlockchain)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;

    for (Index = 1; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {
        /* delete network with fiscobcos interface, but the protocol type don't macth fiscobcos  */
        rtnVal = BoATFiscobcosNetworkDelete(Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);

        /* protocol type of network which index equal 2 is platon*/
        if (Index != 2)
        {
            /* delete network with venachain interface, but the protocol type don't macth venachain*/
            rtnVal = BoATVenachainNetworkDelete(Index);
            /* check the result of getting network data,There has no Venachain network */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }
        /* protocol type of network which index equal 1 is eth*/
        if (Index != 1)
        {
            /* delete network with quorum interface, but the protocol type don't macth quorum*/
            rtnVal = BoATQuorumNetworkDelete(Index);
            /* check the result of getting network data */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }
        /* protocol type of network which index equal 3 is platone*/
        if (Index != 3)
        {
            /* delete network with hwbcs interface, but the protocol type don't macth hwbcs*/
            rtnVal = BoATHwbcsNetworkDelete(Index);
            /* check the result of getting network data*/
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }
        /* protocol type of network which index equal 4 is fiscobcos*/
        if (Index != 4)
        {
            /* delete network with chainmaker interface, but the protocol type don't macth chainmaker*/
            rtnVal = BoATChainmakerNetworkDelete(Index);
            /* check the result of getting network data */
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }
        /* protocol type of network which index equal 5 is fabric*/
        if (Index != 5)
        {
            /* delete network with cita interface, but the protocol type don't macth cita*/
            rtnVal = BoATCitaNetworkDelete(Index);
            /* check the result of getting network data*/
            ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        }
        /* delete network with platone interface, but the protocol type don't macth platone  */
        rtnVal = BoATPlatoneNetworkDelete(Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* delete network with Eth interface, but the protocol type don't macth Eth  */
        rtnVal = BoATEthNetworkDelete(Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* delete network with fabric interface, but the protocol type don't macth fabric  */
        rtnVal = BoATHlfabricNetworkDelete(Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
        /* delete network with PlatON interface, but the protocol type don't macth PlatON */
        rtnVal = BoATPlatONNetworkDelete(Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete network by index using right interface;
* @return
****************************************************************************************
*/
START_TEST(test_002CrossNetwork_0020DeleteNetwork_Success)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;

    for (Index = 1; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {

        /* protocol type of network which index equal 2 is Venachain*/
        if (Index == 2)
        {
            /* delete network with Venachain interface, the protocol type macth Venachain */
            rtnVal = BoATVenachainNetworkDelete(Index);
            /* check the result of getting network data */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        /* protocol type of network which index equal 1 is Quorum*/
        if (Index == 1)
        {
            /* delete network with Quorum interface, the protocol type macth Quorum  */
            rtnVal = BoATQuorumNetworkDelete(Index);
            /* check the result of getting network data */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        /* protocol type of network which index equal 3 is hwbcs*/
        if (Index == 3)
        {
            /* delete network with hwbcs interface, the protocol type macth hwbcs  */
            rtnVal = BoATHwbcsNetworkDelete(Index);
            /* check the result of getting network data*/
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        /* protocol type of network which index equal 4 is Chainmaker*/
        if (Index == 4)
        {
            /* delete network with Chainmaker interface, the protocol type macth Chainmaker  */
            rtnVal = BoATChainmakerNetworkDelete(Index);
            /* check the result of getting network data */
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
        /* protocol type of network which index equal 5 is Cita*/
        if (Index == 5)
        {
            /* delete network with Cita interface, the protocol type macth Cita  */
            rtnVal = BoATCitaNetworkDelete(Index);
            /* check the result of getting network data*/
            ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        }
    }
}
END_TEST

Suite *make_NetworkCrossUtTest_suite(void)
{
    /* Create Suite */
    Suite *s_networkcross = suite_create("networkCross");

    /* Create test cases */
    TCase *tc_networkCross_api = tcase_create("networkCross_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_networkcross, tc_networkCross_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0001CreateOneTimeNetworkSuccess_ETH);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0002CreateOneTimeNetworkSuccess_platON);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0003CreateOneTimeNetworkSuccess_platone);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0004CreateOneTimeNetworkSuccess_fiscobcos);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0005CreateOneTimeNetworkSuccess_fabric);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0006CreateOneTimeNetworkSuccess_hwbcs);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0007CreateOneTimeNetworkSuccess_quorum);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0008CreateOneTimeNetworkSuccess_venachain);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0009CreateOneTimeNetworkSuccess_cita);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0010CreateOneTimeNetworkSuccess_chainmaker);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0011Create5PersistNetworkSuccess);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0012Create6thPersistNetworkFail);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0013GetNetworkbyIndexSuccess);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0014DeleteNetwork_otherBlockchain);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0015DeleteNetwork_Success);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0016CreatOtherNetworks_Success);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0017CreateAnother6thPersistNetworkFail);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0018GetNetworkbyIndexSuccess);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0019DeleteNetwork_otherBlockchain);
    tcase_add_test(tc_networkCross_api, test_002CrossNetwork_0020DeleteNetwork_Success);

    return s_networkcross;
}