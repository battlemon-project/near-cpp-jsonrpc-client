// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: auth.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_auth_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_auth_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_auth_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_auth_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[4]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_auth_2eproto;
namespace game {
namespace battlemon {
namespace auth {
class SendCodeRequest;
struct SendCodeRequestDefaultTypeInternal;
extern SendCodeRequestDefaultTypeInternal _SendCodeRequest_default_instance_;
class SendCodeResponse;
struct SendCodeResponseDefaultTypeInternal;
extern SendCodeResponseDefaultTypeInternal _SendCodeResponse_default_instance_;
class VerifyCodeRequest;
struct VerifyCodeRequestDefaultTypeInternal;
extern VerifyCodeRequestDefaultTypeInternal _VerifyCodeRequest_default_instance_;
class VerifyCodeResponse;
struct VerifyCodeResponseDefaultTypeInternal;
extern VerifyCodeResponseDefaultTypeInternal _VerifyCodeResponse_default_instance_;
}  // namespace auth
}  // namespace battlemon
}  // namespace game
PROTOBUF_NAMESPACE_OPEN
template<> ::game::battlemon::auth::SendCodeRequest* Arena::CreateMaybeMessage<::game::battlemon::auth::SendCodeRequest>(Arena*);
template<> ::game::battlemon::auth::SendCodeResponse* Arena::CreateMaybeMessage<::game::battlemon::auth::SendCodeResponse>(Arena*);
template<> ::game::battlemon::auth::VerifyCodeRequest* Arena::CreateMaybeMessage<::game::battlemon::auth::VerifyCodeRequest>(Arena*);
template<> ::game::battlemon::auth::VerifyCodeResponse* Arena::CreateMaybeMessage<::game::battlemon::auth::VerifyCodeResponse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace game {
namespace battlemon {
namespace auth {

// ===================================================================

class SendCodeRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:game.battlemon.auth.SendCodeRequest) */ {
 public:
  inline SendCodeRequest() : SendCodeRequest(nullptr) {}
  ~SendCodeRequest() override;
  explicit constexpr SendCodeRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  SendCodeRequest(const SendCodeRequest& from);
  SendCodeRequest(SendCodeRequest&& from) noexcept
    : SendCodeRequest() {
    *this = ::std::move(from);
  }

  inline SendCodeRequest& operator=(const SendCodeRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline SendCodeRequest& operator=(SendCodeRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const SendCodeRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const SendCodeRequest* internal_default_instance() {
    return reinterpret_cast<const SendCodeRequest*>(
               &_SendCodeRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SendCodeRequest& a, SendCodeRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(SendCodeRequest* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SendCodeRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  SendCodeRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<SendCodeRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const SendCodeRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const SendCodeRequest& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(SendCodeRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "game.battlemon.auth.SendCodeRequest";
  }
  protected:
  explicit SendCodeRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kPublicKeyFieldNumber = 1,
  };
  // string public_key = 1;
  void clear_public_key();
  const std::string& public_key() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_public_key(ArgT0&& arg0, ArgT... args);
  std::string* mutable_public_key();
  PROTOBUF_NODISCARD std::string* release_public_key();
  void set_allocated_public_key(std::string* public_key);
  private:
  const std::string& _internal_public_key() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_public_key(const std::string& value);
  std::string* _internal_mutable_public_key();
  public:

  // @@protoc_insertion_point(class_scope:game.battlemon.auth.SendCodeRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr public_key_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_auth_2eproto;
};
// -------------------------------------------------------------------

class SendCodeResponse final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:game.battlemon.auth.SendCodeResponse) */ {
 public:
  inline SendCodeResponse() : SendCodeResponse(nullptr) {}
  ~SendCodeResponse() override;
  explicit constexpr SendCodeResponse(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  SendCodeResponse(const SendCodeResponse& from);
  SendCodeResponse(SendCodeResponse&& from) noexcept
    : SendCodeResponse() {
    *this = ::std::move(from);
  }

  inline SendCodeResponse& operator=(const SendCodeResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline SendCodeResponse& operator=(SendCodeResponse&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const SendCodeResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const SendCodeResponse* internal_default_instance() {
    return reinterpret_cast<const SendCodeResponse*>(
               &_SendCodeResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(SendCodeResponse& a, SendCodeResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(SendCodeResponse* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SendCodeResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  SendCodeResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<SendCodeResponse>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const SendCodeResponse& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const SendCodeResponse& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(SendCodeResponse* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "game.battlemon.auth.SendCodeResponse";
  }
  protected:
  explicit SendCodeResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kCodeFieldNumber = 1,
  };
  // string code = 1;
  void clear_code();
  const std::string& code() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_code(ArgT0&& arg0, ArgT... args);
  std::string* mutable_code();
  PROTOBUF_NODISCARD std::string* release_code();
  void set_allocated_code(std::string* code);
  private:
  const std::string& _internal_code() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_code(const std::string& value);
  std::string* _internal_mutable_code();
  public:

  // @@protoc_insertion_point(class_scope:game.battlemon.auth.SendCodeResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr code_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_auth_2eproto;
};
// -------------------------------------------------------------------

class VerifyCodeRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:game.battlemon.auth.VerifyCodeRequest) */ {
 public:
  inline VerifyCodeRequest() : VerifyCodeRequest(nullptr) {}
  ~VerifyCodeRequest() override;
  explicit constexpr VerifyCodeRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  VerifyCodeRequest(const VerifyCodeRequest& from);
  VerifyCodeRequest(VerifyCodeRequest&& from) noexcept
    : VerifyCodeRequest() {
    *this = ::std::move(from);
  }

  inline VerifyCodeRequest& operator=(const VerifyCodeRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline VerifyCodeRequest& operator=(VerifyCodeRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const VerifyCodeRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const VerifyCodeRequest* internal_default_instance() {
    return reinterpret_cast<const VerifyCodeRequest*>(
               &_VerifyCodeRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(VerifyCodeRequest& a, VerifyCodeRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(VerifyCodeRequest* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(VerifyCodeRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  VerifyCodeRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<VerifyCodeRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const VerifyCodeRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const VerifyCodeRequest& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(VerifyCodeRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "game.battlemon.auth.VerifyCodeRequest";
  }
  protected:
  explicit VerifyCodeRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kPublicKeyFieldNumber = 1,
    kSignFieldNumber = 2,
  };
  // string public_key = 1;
  void clear_public_key();
  const std::string& public_key() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_public_key(ArgT0&& arg0, ArgT... args);
  std::string* mutable_public_key();
  PROTOBUF_NODISCARD std::string* release_public_key();
  void set_allocated_public_key(std::string* public_key);
  private:
  const std::string& _internal_public_key() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_public_key(const std::string& value);
  std::string* _internal_mutable_public_key();
  public:

  // string sign = 2;
  void clear_sign();
  const std::string& sign() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_sign(ArgT0&& arg0, ArgT... args);
  std::string* mutable_sign();
  PROTOBUF_NODISCARD std::string* release_sign();
  void set_allocated_sign(std::string* sign);
  private:
  const std::string& _internal_sign() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_sign(const std::string& value);
  std::string* _internal_mutable_sign();
  public:

  // @@protoc_insertion_point(class_scope:game.battlemon.auth.VerifyCodeRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr public_key_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr sign_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_auth_2eproto;
};
// -------------------------------------------------------------------

class VerifyCodeResponse final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:game.battlemon.auth.VerifyCodeResponse) */ {
 public:
  inline VerifyCodeResponse() : VerifyCodeResponse(nullptr) {}
  ~VerifyCodeResponse() override;
  explicit constexpr VerifyCodeResponse(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  VerifyCodeResponse(const VerifyCodeResponse& from);
  VerifyCodeResponse(VerifyCodeResponse&& from) noexcept
    : VerifyCodeResponse() {
    *this = ::std::move(from);
  }

  inline VerifyCodeResponse& operator=(const VerifyCodeResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline VerifyCodeResponse& operator=(VerifyCodeResponse&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const VerifyCodeResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const VerifyCodeResponse* internal_default_instance() {
    return reinterpret_cast<const VerifyCodeResponse*>(
               &_VerifyCodeResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  friend void swap(VerifyCodeResponse& a, VerifyCodeResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(VerifyCodeResponse* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(VerifyCodeResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  VerifyCodeResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<VerifyCodeResponse>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const VerifyCodeResponse& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const VerifyCodeResponse& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(VerifyCodeResponse* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "game.battlemon.auth.VerifyCodeResponse";
  }
  protected:
  explicit VerifyCodeResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNearAccountIdFieldNumber = 1,
  };
  // string near_account_id = 1;
  void clear_near_account_id();
  const std::string& near_account_id() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_near_account_id(ArgT0&& arg0, ArgT... args);
  std::string* mutable_near_account_id();
  PROTOBUF_NODISCARD std::string* release_near_account_id();
  void set_allocated_near_account_id(std::string* near_account_id);
  private:
  const std::string& _internal_near_account_id() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_near_account_id(const std::string& value);
  std::string* _internal_mutable_near_account_id();
  public:

  // @@protoc_insertion_point(class_scope:game.battlemon.auth.VerifyCodeResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr near_account_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_auth_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SendCodeRequest

// string public_key = 1;
inline void SendCodeRequest::clear_public_key() {
  public_key_.ClearToEmpty();
}
inline const std::string& SendCodeRequest::public_key() const {
  // @@protoc_insertion_point(field_get:game.battlemon.auth.SendCodeRequest.public_key)
  return _internal_public_key();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void SendCodeRequest::set_public_key(ArgT0&& arg0, ArgT... args) {
 
 public_key_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:game.battlemon.auth.SendCodeRequest.public_key)
}
inline std::string* SendCodeRequest::mutable_public_key() {
  std::string* _s = _internal_mutable_public_key();
  // @@protoc_insertion_point(field_mutable:game.battlemon.auth.SendCodeRequest.public_key)
  return _s;
}
inline const std::string& SendCodeRequest::_internal_public_key() const {
  return public_key_.Get();
}
inline void SendCodeRequest::_internal_set_public_key(const std::string& value) {
  
  public_key_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* SendCodeRequest::_internal_mutable_public_key() {
  
  return public_key_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* SendCodeRequest::release_public_key() {
  // @@protoc_insertion_point(field_release:game.battlemon.auth.SendCodeRequest.public_key)
  return public_key_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void SendCodeRequest::set_allocated_public_key(std::string* public_key) {
  if (public_key != nullptr) {
    
  } else {
    
  }
  public_key_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), public_key,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (public_key_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    public_key_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:game.battlemon.auth.SendCodeRequest.public_key)
}

// -------------------------------------------------------------------

// SendCodeResponse

// string code = 1;
inline void SendCodeResponse::clear_code() {
  code_.ClearToEmpty();
}
inline const std::string& SendCodeResponse::code() const {
  // @@protoc_insertion_point(field_get:game.battlemon.auth.SendCodeResponse.code)
  return _internal_code();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void SendCodeResponse::set_code(ArgT0&& arg0, ArgT... args) {
 
 code_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:game.battlemon.auth.SendCodeResponse.code)
}
inline std::string* SendCodeResponse::mutable_code() {
  std::string* _s = _internal_mutable_code();
  // @@protoc_insertion_point(field_mutable:game.battlemon.auth.SendCodeResponse.code)
  return _s;
}
inline const std::string& SendCodeResponse::_internal_code() const {
  return code_.Get();
}
inline void SendCodeResponse::_internal_set_code(const std::string& value) {
  
  code_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* SendCodeResponse::_internal_mutable_code() {
  
  return code_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* SendCodeResponse::release_code() {
  // @@protoc_insertion_point(field_release:game.battlemon.auth.SendCodeResponse.code)
  return code_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void SendCodeResponse::set_allocated_code(std::string* code) {
  if (code != nullptr) {
    
  } else {
    
  }
  code_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), code,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (code_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    code_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:game.battlemon.auth.SendCodeResponse.code)
}

// -------------------------------------------------------------------

// VerifyCodeRequest

// string public_key = 1;
inline void VerifyCodeRequest::clear_public_key() {
  public_key_.ClearToEmpty();
}
inline const std::string& VerifyCodeRequest::public_key() const {
  // @@protoc_insertion_point(field_get:game.battlemon.auth.VerifyCodeRequest.public_key)
  return _internal_public_key();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void VerifyCodeRequest::set_public_key(ArgT0&& arg0, ArgT... args) {
 
 public_key_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:game.battlemon.auth.VerifyCodeRequest.public_key)
}
inline std::string* VerifyCodeRequest::mutable_public_key() {
  std::string* _s = _internal_mutable_public_key();
  // @@protoc_insertion_point(field_mutable:game.battlemon.auth.VerifyCodeRequest.public_key)
  return _s;
}
inline const std::string& VerifyCodeRequest::_internal_public_key() const {
  return public_key_.Get();
}
inline void VerifyCodeRequest::_internal_set_public_key(const std::string& value) {
  
  public_key_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* VerifyCodeRequest::_internal_mutable_public_key() {
  
  return public_key_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* VerifyCodeRequest::release_public_key() {
  // @@protoc_insertion_point(field_release:game.battlemon.auth.VerifyCodeRequest.public_key)
  return public_key_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void VerifyCodeRequest::set_allocated_public_key(std::string* public_key) {
  if (public_key != nullptr) {
    
  } else {
    
  }
  public_key_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), public_key,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (public_key_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    public_key_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:game.battlemon.auth.VerifyCodeRequest.public_key)
}

// string sign = 2;
inline void VerifyCodeRequest::clear_sign() {
  sign_.ClearToEmpty();
}
inline const std::string& VerifyCodeRequest::sign() const {
  // @@protoc_insertion_point(field_get:game.battlemon.auth.VerifyCodeRequest.sign)
  return _internal_sign();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void VerifyCodeRequest::set_sign(ArgT0&& arg0, ArgT... args) {
 
 sign_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:game.battlemon.auth.VerifyCodeRequest.sign)
}
inline std::string* VerifyCodeRequest::mutable_sign() {
  std::string* _s = _internal_mutable_sign();
  // @@protoc_insertion_point(field_mutable:game.battlemon.auth.VerifyCodeRequest.sign)
  return _s;
}
inline const std::string& VerifyCodeRequest::_internal_sign() const {
  return sign_.Get();
}
inline void VerifyCodeRequest::_internal_set_sign(const std::string& value) {
  
  sign_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* VerifyCodeRequest::_internal_mutable_sign() {
  
  return sign_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* VerifyCodeRequest::release_sign() {
  // @@protoc_insertion_point(field_release:game.battlemon.auth.VerifyCodeRequest.sign)
  return sign_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void VerifyCodeRequest::set_allocated_sign(std::string* sign) {
  if (sign != nullptr) {
    
  } else {
    
  }
  sign_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), sign,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (sign_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    sign_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:game.battlemon.auth.VerifyCodeRequest.sign)
}

// -------------------------------------------------------------------

// VerifyCodeResponse

// string near_account_id = 1;
inline void VerifyCodeResponse::clear_near_account_id() {
  near_account_id_.ClearToEmpty();
}
inline const std::string& VerifyCodeResponse::near_account_id() const {
  // @@protoc_insertion_point(field_get:game.battlemon.auth.VerifyCodeResponse.near_account_id)
  return _internal_near_account_id();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void VerifyCodeResponse::set_near_account_id(ArgT0&& arg0, ArgT... args) {
 
 near_account_id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:game.battlemon.auth.VerifyCodeResponse.near_account_id)
}
inline std::string* VerifyCodeResponse::mutable_near_account_id() {
  std::string* _s = _internal_mutable_near_account_id();
  // @@protoc_insertion_point(field_mutable:game.battlemon.auth.VerifyCodeResponse.near_account_id)
  return _s;
}
inline const std::string& VerifyCodeResponse::_internal_near_account_id() const {
  return near_account_id_.Get();
}
inline void VerifyCodeResponse::_internal_set_near_account_id(const std::string& value) {
  
  near_account_id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* VerifyCodeResponse::_internal_mutable_near_account_id() {
  
  return near_account_id_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* VerifyCodeResponse::release_near_account_id() {
  // @@protoc_insertion_point(field_release:game.battlemon.auth.VerifyCodeResponse.near_account_id)
  return near_account_id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void VerifyCodeResponse::set_allocated_near_account_id(std::string* near_account_id) {
  if (near_account_id != nullptr) {
    
  } else {
    
  }
  near_account_id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), near_account_id,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (near_account_id_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    near_account_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:game.battlemon.auth.VerifyCodeResponse.near_account_id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace auth
}  // namespace battlemon
}  // namespace game

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_auth_2eproto
