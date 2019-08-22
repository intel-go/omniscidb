/*
 * Copyright 2017 MapD Technologies, Inc.
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
 */
#ifndef QUERYENGINE_JOINHASHTABLEINTERFACE_H
#define QUERYENGINE_JOINHASHTABLEINTERFACE_H

#include <llvm/IR/Value.h>
#include <cstdint>
#include "CompilationOptions.h"
#include "Descriptors/InputDescriptors.h"

class TooManyHashEntries : public std::runtime_error {
 public:
  TooManyHashEntries()
      : std::runtime_error("Hash tables with more than 2B entries not supported yet") {}
};

class TableMustBeReplicated : public std::runtime_error {
 public:
  TableMustBeReplicated(const std::string& table_name)
      : std::runtime_error("Hash join failed: Table '" + table_name +
                           "' must be replicated.") {}
};

class HashJoinFail : public std::runtime_error {
 public:
  HashJoinFail(const std::string& reason) : std::runtime_error(reason) {}
};

class FailedToFetchColumn : public HashJoinFail {
 public:
  FailedToFetchColumn()
      : HashJoinFail("Not enough memory for columns involvde in join") {}
};

class FailedToJoinOnVirtualColumn : public HashJoinFail {
 public:
  FailedToJoinOnVirtualColumn() : HashJoinFail("Cannot join on rowid") {}
};

struct HashJoinMatchingSet {
  llvm::Value* elements;
  llvm::Value* count;
  llvm::Value* slot;
};

using InnerOuter = std::pair<const Analyzer::ColumnVar*, const Analyzer::Expr*>;
using InputColDescriptors = std::list<std::shared_ptr<const InputColDescriptor>>;
using InputColDescriptorsByScanIdx = std::unordered_map<int, InputColDescriptors>;

class JoinHashTableInterface {
 public:
  virtual int64_t getJoinHashBuffer(const ExecutorDeviceType device_type,
                                    const int device_id) noexcept = 0;

  virtual llvm::Value* codegenSlot(const CompilationOptions&, const size_t) = 0;

  virtual HashJoinMatchingSet codegenMatchingSet(const CompilationOptions&,
                                                 const size_t) = 0;

  virtual int getInnerTableId() const noexcept = 0;

  virtual int getInnerTableRteIdx() const noexcept = 0;

  enum class HashType { OneToOne, OneToMany };

  virtual HashType getHashType() const noexcept = 0;

  enum class PayloadType { RowId, RowIdAndRow, Row };

  virtual PayloadType getPayloadType() const noexcept { return PayloadType::RowId; }

  virtual size_t getPayloadSize() const noexcept { return 1; }

  virtual const InputColDescriptors& getPayloadColumns() const noexcept {
    return EMPTY_PAYLOAD;
  }

  virtual size_t getPayloadColumnOffset(const InputColDescriptor& col) const noexcept {
    CHECK(false);
    return 0;
  }

  virtual llvm::Value* getPayloadColumnPtr(const InputColDescriptor& col) const noexcept {
    CHECK(false);
    return nullptr;
  }

  virtual size_t offsetBufferOff() const noexcept = 0;

  virtual size_t countBufferOff() const noexcept = 0;

  virtual size_t payloadBufferOff() const noexcept = 0;

 private:
  static const InputColDescriptors EMPTY_PAYLOAD;
};

#endif  // QUERYENGINE_JOINHASHTABLEINTERFACE_H
