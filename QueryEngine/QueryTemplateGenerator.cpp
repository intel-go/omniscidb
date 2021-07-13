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

#include "QueryTemplateGenerator.h"
#include "IRCodegenUtils.h"
#include "Logger/Logger.h"

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Verifier.h>

// This file was pretty much auto-generated by running:
//      llc -march=cpp RuntimeFunctions.ll
// and formatting the results to be more readable.

namespace {

inline llvm::Type* get_pointer_element_type(llvm::Value* value) {
  CHECK(value);
  auto type = value->getType();
  CHECK(type && type->isPointerTy());
  auto pointer_type = llvm::dyn_cast<llvm::PointerType>(type);
  CHECK(pointer_type);
  return pointer_type->getElementType();
}

template <class Attributes>
llvm::Function* default_func_builder(llvm::Module* mod,
                                     const std::string& name,
                                     llvm::CallingConv::ID CC) {
  using namespace llvm;

  std::vector<Type*> func_args;
  FunctionType* func_type = FunctionType::get(
      /*Result=*/IntegerType::get(mod->getContext(), 32),
      /*Params=*/func_args,
      /*isVarArg=*/false);

  auto func_ptr = mod->getFunction(name);
  if (!func_ptr) {
    func_ptr = Function::Create(
        /*Type=*/func_type,
        /*Linkage=*/GlobalValue::ExternalLinkage,
        /*Name=*/name,
        mod);  // (external, no body)
    func_ptr->setCallingConv(CC);
  }

  Attributes func_pal;
  {
    SmallVector<Attributes, 4> Attrs;
    Attributes PAS;
    {
      AttrBuilder B;
      PAS = Attributes::get(mod->getContext(), ~0U, B);
    }

    Attrs.push_back(PAS);
    func_pal = Attributes::get(mod->getContext(), Attrs);
  }
  func_ptr->setAttributes(func_pal);

  return func_ptr;
}

template <class Attributes>
llvm::Function* pos_start(llvm::Module* mod,
                          llvm::CallingConv::ID CC = llvm::CallingConv::C) {
  return default_func_builder<Attributes>(mod, "pos_start", CC);
}

template <class Attributes>
llvm::Function* group_buff_idx(llvm::Module* mod,
                               llvm::CallingConv::ID CC = llvm::CallingConv::C) {
  return default_func_builder<Attributes>(mod, "group_buff_idx", CC);
}

template <class Attributes>
llvm::Function* pos_step(llvm::Module* mod,
                         llvm::CallingConv::ID CC = llvm::CallingConv::C) {
  using namespace llvm;

  std::vector<Type*> func_args;
  FunctionType* func_type = FunctionType::get(
      /*Result=*/IntegerType::get(mod->getContext(), 32),
      /*Params=*/func_args,
      /*isVarArg=*/false);

  auto func_ptr = mod->getFunction("pos_step");
  if (!func_ptr) {
    func_ptr = Function::Create(
        /*Type=*/func_type,
        /*Linkage=*/GlobalValue::ExternalLinkage,
        /*Name=*/"pos_step",
        mod);  // (external, no body)
    func_ptr->setCallingConv(CC);
  }

  Attributes func_pal;
  {
    SmallVector<Attributes, 4> Attrs;
    Attributes PAS;
    {
      AttrBuilder B;
      PAS = Attributes::get(mod->getContext(), ~0U, B);
    }

    Attrs.push_back(PAS);
    func_pal = Attributes::get(mod->getContext(), Attrs);
  }
  func_ptr->setAttributes(func_pal);

  return func_ptr;
}

template <class Attributes>
llvm::Function* row_process(llvm::Module* mod,
                            const size_t aggr_col_count,
                            const bool hoist_literals,
                            llvm::CallingConv::ID CC = llvm::CallingConv::C,
                            unsigned int AS = 0) {
  using namespace llvm;

  std::vector<Type*> func_args;
  auto i8_type = IntegerType::get(mod->getContext(), 8);
  auto i32_type = IntegerType::get(mod->getContext(), 32);
  auto i64_type = IntegerType::get(mod->getContext(), 64);
  auto pi32_type = PointerType::get(i32_type, AS);
  auto pi64_type = PointerType::get(i64_type, AS);
  auto pi64_stack_type = PointerType::get(i64_type, 0);

  if (aggr_col_count) {
    for (size_t i = 0; i < aggr_col_count; ++i) {
      func_args.push_back(pi64_stack_type);
    }
  } else {                           // group by query
    func_args.push_back(pi64_type);  // groups buffer
    func_args.push_back(pi32_type);  // 1 iff current row matched, else 0
    func_args.push_back(pi32_type);  // total rows matched from the caller
    func_args.push_back(pi32_type);  // total rows matched before atomic increment
    func_args.push_back(pi32_type);  // max number of slots in the output buffer
  }

  func_args.push_back(pi64_type);  // aggregate init values

  func_args.push_back(i64_type);
  func_args.push_back(pi64_type);
  func_args.push_back(pi64_type);
  if (hoist_literals) {
    func_args.push_back(PointerType::get(i8_type, AS));
  }
  FunctionType* func_type = FunctionType::get(
      /*Result=*/i32_type,
      /*Params=*/func_args,
      /*isVarArg=*/false);

  std::string func_name{"row_process"};
  auto func_ptr = mod->getFunction(func_name);

  if (!func_ptr) {
    func_ptr = Function::Create(
        /*Type=*/func_type,
        /*Linkage=*/GlobalValue::ExternalLinkage,
        /*Name=*/func_name,
        mod);  // (external, no body)
    func_ptr->setCallingConv(CC);

    Attributes func_pal;
    {
      SmallVector<Attributes, 4> Attrs;
      Attributes PAS;
      {
        AttrBuilder B;
        PAS = Attributes::get(mod->getContext(), ~0U, B);
      }

      Attrs.push_back(PAS);
      func_pal = Attributes::get(mod->getContext(), Attrs);
    }
    func_ptr->setAttributes(func_pal);
  }

  return func_ptr;
}

}  // namespace

template <class Attributes>
std::tuple<llvm::Function*, llvm::CallInst*> query_template_impl(
    llvm::Module* mod,
    const size_t aggr_col_count,
    const CompilationOptions& co,
    const bool is_estimate_query,
    const GpuSharedMemoryContext& gpu_smem_context) {
  using namespace llvm;
  auto calling_conv = (co.device_type == ExecutorDeviceType::L0) ? CallingConv::SPIR_FUNC
                                                                 : CallingConv::C;
  unsigned int addr_space = (co.device_type == ExecutorDeviceType::L0) ? 4 : 0;

  auto func_pos_start = pos_start<Attributes>(mod, calling_conv);
  CHECK(func_pos_start);
  auto func_pos_step = pos_step<Attributes>(mod, calling_conv);
  CHECK(func_pos_step);
  auto func_group_buff_idx = group_buff_idx<Attributes>(mod, calling_conv);
  CHECK(func_group_buff_idx);
  auto func_row_process = row_process<Attributes>(mod,
                                                  is_estimate_query ? 1 : aggr_col_count,
                                                  co.hoist_literals,
                                                  calling_conv,
                                                  addr_space);
  CHECK(func_row_process);

  auto i8_type = IntegerType::get(mod->getContext(), 8);
  auto i32_type = IntegerType::get(mod->getContext(), 32);
  auto i64_type = IntegerType::get(mod->getContext(), 64);
  auto pi8_type = PointerType::get(i8_type, addr_space);
  auto ppi8_type = PointerType::get(pi8_type, addr_space);
  auto pi32_type = PointerType::get(i32_type, addr_space);
  auto pi64_type = PointerType::get(i64_type, addr_space);
  auto ppi64_type = PointerType::get(pi64_type, addr_space);

  auto byte_stream_type =
      (co.device_type == ExecutorDeviceType::L0) ? pi8_type : ppi8_type;
  auto out_type = (co.device_type == ExecutorDeviceType::L0) ? pi64_type : ppi64_type;

  std::vector<Type*> query_args;
  query_args.push_back(ppi8_type);
  if (co.hoist_literals) {
    query_args.push_back(pi8_type);
  }
  query_args.push_back(pi64_type);
  query_args.push_back(pi64_type);
  query_args.push_back(pi32_type);

  query_args.push_back(pi64_type);
  query_args.push_back(ppi64_type);
  query_args.push_back(i32_type);
  query_args.push_back(pi64_type);
  query_args.push_back(pi32_type);
  query_args.push_back(pi32_type);

  FunctionType* query_func_type = FunctionType::get(
      /*Result=*/Type::getVoidTy(mod->getContext()),
      /*Params=*/query_args,
      /*isVarArg=*/false);

  std::string query_template_name{"query_template"};
  auto query_func_ptr = mod->getFunction(query_template_name);
  CHECK(!query_func_ptr);

  query_func_ptr = Function::Create(
      /*Type=*/query_func_type,
      /*Linkage=*/GlobalValue::ExternalLinkage,
      /*Name=*/query_template_name,
      mod);
  query_func_ptr->setCallingConv(calling_conv);

  Attributes query_func_pal;
  {
    SmallVector<Attributes, 4> Attrs;
    Attributes PAS;
    {
      AttrBuilder B;
      B.addAttribute(Attribute::NoCapture);
      PAS = Attributes::get(mod->getContext(), 1U, B);
    }

    Attrs.push_back(PAS);
    {
      AttrBuilder B;
      B.addAttribute(Attribute::NoCapture);
      PAS = Attributes::get(mod->getContext(), 2U, B);
    }

    Attrs.push_back(PAS);

    {
      AttrBuilder B;
      B.addAttribute(Attribute::NoCapture);
      Attrs.push_back(Attributes::get(mod->getContext(), 3U, B));
    }

    {
      AttrBuilder B;
      B.addAttribute(Attribute::NoCapture);
      Attrs.push_back(Attributes::get(mod->getContext(), 4U, B));
    }

    Attrs.push_back(PAS);

    query_func_pal = Attributes::get(mod->getContext(), Attrs);
  }
  query_func_ptr->setAttributes(query_func_pal);

  Function::arg_iterator query_arg_it = query_func_ptr->arg_begin();
  Value* byte_stream = &*query_arg_it;
  byte_stream->setName("byte_stream");
  Value* literals{nullptr};
  if (co.hoist_literals) {
    literals = &*(++query_arg_it);
    literals->setName("literals");
  }
  Value* row_count_ptr = &*(++query_arg_it);
  row_count_ptr->setName("row_count_ptr");
  Value* frag_row_off_ptr = &*(++query_arg_it);
  frag_row_off_ptr->setName("frag_row_off_ptr");
  Value* max_matched_ptr = &*(++query_arg_it);
  max_matched_ptr->setName("max_matched_ptr");
  Value* agg_init_val = &*(++query_arg_it);
  agg_init_val->setName("agg_init_val");
  Value* out = &*(++query_arg_it);
  out->setName("out");
  Value* frag_idx = &*(++query_arg_it);
  frag_idx->setName("frag_idx");
  Value* join_hash_tables = &*(++query_arg_it);
  join_hash_tables->setName("join_hash_tables");
  Value* total_matched = &*(++query_arg_it);
  total_matched->setName("total_matched");
  Value* error_code = &*(++query_arg_it);
  error_code->setName("error_code");

  auto bb_entry = BasicBlock::Create(mod->getContext(), ".entry", query_func_ptr, 0);
  auto bb_preheader =
      BasicBlock::Create(mod->getContext(), ".loop.preheader", query_func_ptr, 0);
  auto bb_forbody = BasicBlock::Create(mod->getContext(), ".for.body", query_func_ptr, 0);
  auto bb_crit_edge =
      BasicBlock::Create(mod->getContext(), "._crit_edge", query_func_ptr, 0);
  auto bb_exit = BasicBlock::Create(mod->getContext(), ".exit", query_func_ptr, 0);

  // Block  (.entry)
  std::vector<Value*> result_ptr_vec;
  llvm::CallInst* smem_output_buffer{nullptr};
  if (!is_estimate_query) {
    for (size_t i = 0; i < aggr_col_count; ++i) {
      auto result_ptr = new AllocaInst(i64_type, 0, "result", bb_entry);
      result_ptr->setAlignment(LLVM_ALIGN(8));
      result_ptr_vec.push_back(result_ptr);
    }
    if (gpu_smem_context.isSharedMemoryUsed()) {
      auto init_smem_func = mod->getFunction("init_shared_mem");
      CHECK(init_smem_func);
      // only one slot per aggregate column is needed, and so we can initialize shared
      // memory buffer for intermediate results to be exactly like the agg_init_val array
      smem_output_buffer = CallInst::Create(
          init_smem_func,
          std::vector<llvm::Value*>{
              agg_init_val,
              llvm::ConstantInt::get(i32_type, aggr_col_count * sizeof(int64_t))},
          "smem_buffer",
          bb_entry);
    }
  }

  LoadInst* row_count = new LoadInst(get_pointer_element_type(row_count_ptr),
                                     row_count_ptr,
                                     "row_count",
                                     false,
                                     bb_entry);
  row_count->setAlignment(LLVM_ALIGN(8));
  row_count->setName("row_count");
  std::vector<Value*> agg_init_val_vec;
  if (!is_estimate_query) {
    for (size_t i = 0; i < aggr_col_count; ++i) {
      auto idx_lv = ConstantInt::get(i32_type, i);
      auto agg_init_gep =
          GetElementPtrInst::CreateInBounds(agg_init_val, idx_lv, "", bb_entry);
      auto agg_init_val = new LoadInst(
          get_pointer_element_type(agg_init_gep), agg_init_gep, "", false, bb_entry);
      agg_init_val->setAlignment(LLVM_ALIGN(8));
      agg_init_val_vec.push_back(agg_init_val);
      auto init_val_st = new StoreInst(agg_init_val, result_ptr_vec[i], false, bb_entry);
      init_val_st->setAlignment(LLVM_ALIGN(8));
    }
  }

  CallInst* pos_start = CallInst::Create(func_pos_start, "pos_start", bb_entry);
  pos_start->setCallingConv(calling_conv);
  pos_start->setTailCall(true);
  Attributes pos_start_pal;
  pos_start->setAttributes(pos_start_pal);

  CallInst* pos_step = CallInst::Create(func_pos_step, "pos_step", bb_entry);
  pos_step->setCallingConv(calling_conv);
  pos_step->setTailCall(true);
  Attributes pos_step_pal;
  pos_step->setAttributes(pos_step_pal);

  CallInst* group_buff_idx = nullptr;
  if (!is_estimate_query) {
    group_buff_idx = CallInst::Create(func_group_buff_idx, "group_buff_idx", bb_entry);
    group_buff_idx->setCallingConv(calling_conv);
    group_buff_idx->setTailCall(true);
    Attributes group_buff_idx_pal;
    group_buff_idx->setAttributes(group_buff_idx_pal);
  }

  CastInst* pos_start_i64 = new SExtInst(pos_start, i64_type, "", bb_entry);
  ICmpInst* enter_or_not =
      new ICmpInst(*bb_entry, ICmpInst::ICMP_SLT, pos_start_i64, row_count, "");
  BranchInst::Create(bb_preheader, bb_exit, enter_or_not, bb_entry);

  // Block .loop.preheader
  CastInst* pos_step_i64 = new SExtInst(pos_step, i64_type, "", bb_preheader);
  BranchInst::Create(bb_forbody, bb_preheader);

  // Block  .forbody
  Argument* pos_inc_pre = new Argument(i64_type);
  PHINode* pos = PHINode::Create(i64_type, 2, "pos", bb_forbody);
  pos->addIncoming(pos_start_i64, bb_preheader);
  pos->addIncoming(pos_inc_pre, bb_forbody);

  std::vector<Value*> row_process_params;
  row_process_params.insert(
      row_process_params.end(), result_ptr_vec.begin(), result_ptr_vec.end());
  if (is_estimate_query) {
    row_process_params.push_back(
        new LoadInst(get_pointer_element_type(out), out, "", false, bb_forbody));
  }
  row_process_params.push_back(agg_init_val);
  row_process_params.push_back(pos);
  row_process_params.push_back(frag_row_off_ptr);
  row_process_params.push_back(row_count_ptr);
  if (co.hoist_literals) {
    CHECK(literals);
    row_process_params.push_back(literals);
  }
  CallInst* row_process =
      CallInst::Create(func_row_process, row_process_params, "", bb_forbody);
  row_process->setCallingConv(calling_conv);
  row_process->setTailCall(false);
  Attributes row_process_pal;
  row_process->setAttributes(row_process_pal);

  BinaryOperator* pos_inc =
      BinaryOperator::CreateNSW(Instruction::Add, pos, pos_step_i64, "", bb_forbody);
  ICmpInst* loop_or_exit =
      new ICmpInst(*bb_forbody, ICmpInst::ICMP_SLT, pos_inc, row_count, "");
  BranchInst::Create(bb_forbody, bb_crit_edge, loop_or_exit, bb_forbody);

  // Block ._crit_edge
  std::vector<Instruction*> result_vec_pre;
  if (!is_estimate_query) {
    for (size_t i = 0; i < aggr_col_count; ++i) {
      auto result = new LoadInst(get_pointer_element_type(result_ptr_vec[i]),
                                 result_ptr_vec[i],
                                 ".pre.result",
                                 false,
                                 bb_crit_edge);
      result->setAlignment(LLVM_ALIGN(8));
      result_vec_pre.push_back(result);
    }
  }

  BranchInst::Create(bb_exit, bb_crit_edge);

  // Block  .exit
  /**
   * If GPU shared memory optimization is disabled, for each aggregate target, threads
   * copy back their aggregate results (stored in registers) back into memory. This
   * process is performed per processed fragment. In the host the final results are
   * reduced (per target, for all threads and all fragments).
   *
   * If GPU Shared memory optimization is enabled, we properly (atomically) aggregate all
   * thread's results into memory, which makes the final reduction on host much cheaper.
   * Here, we call a noop dummy write back function which will be properly replaced at
   * runtime depending on the target expressions.
   */
  if (!is_estimate_query) {
    std::vector<PHINode*> result_vec;
    for (int64_t i = aggr_col_count - 1; i >= 0; --i) {
      auto result =
          PHINode::Create(IntegerType::get(mod->getContext(), 64), 2, "", bb_exit);
      result->addIncoming(result_vec_pre[i], bb_crit_edge);
      result->addIncoming(agg_init_val_vec[i], bb_entry);
      result_vec.insert(result_vec.begin(), result);
    }

    for (size_t i = 0; i < aggr_col_count; ++i) {
      auto col_idx = ConstantInt::get(i32_type, i);
      if (gpu_smem_context.isSharedMemoryUsed()) {
        auto target_addr =
            GetElementPtrInst::CreateInBounds(smem_output_buffer, col_idx, "", bb_exit);
        // TODO: generalize this once we want to support other types of aggregate
        // functions besides COUNT.
        auto agg_func = mod->getFunction("agg_sum_shared");
        CHECK(agg_func);
        CallInst::Create(
            agg_func, std::vector<llvm::Value*>{target_addr, result_vec[i]}, "", bb_exit);
      } else {
        auto out_gep = GetElementPtrInst::CreateInBounds(out, col_idx, "", bb_exit);
        auto col_buffer =
            new LoadInst(get_pointer_element_type(out_gep), out_gep, "", false, bb_exit);
        col_buffer->setAlignment(LLVM_ALIGN(8));
        auto slot_idx = BinaryOperator::CreateAdd(
            group_buff_idx,
            BinaryOperator::CreateMul(frag_idx, pos_step, "", bb_exit),
            "",
            bb_exit);
        auto target_addr =
            GetElementPtrInst::CreateInBounds(col_buffer, slot_idx, "", bb_exit);
        StoreInst* result_st = new StoreInst(result_vec[i], target_addr, false, bb_exit);
        result_st->setAlignment(LLVM_ALIGN(8));
      }
    }
    if (gpu_smem_context.isSharedMemoryUsed()) {
      // final reduction of results from shared memory buffer back into global memory.
      auto sync_thread_func = mod->getFunction("sync_threadblock");
      CHECK(sync_thread_func);
      CallInst::Create(sync_thread_func, std::vector<llvm::Value*>{}, "", bb_exit);
      auto reduce_smem_to_gmem_func = mod->getFunction("write_back_non_grouped_agg");
      CHECK(reduce_smem_to_gmem_func);
      // each thread reduce the aggregate target corresponding to its own thread ID.
      // If there are more targets than threads we do not currently use shared memory
      // optimization. This can be relaxed if necessary
      for (size_t i = 0; i < aggr_col_count; i++) {
        auto out_gep = GetElementPtrInst::CreateInBounds(
            out, ConstantInt::get(i32_type, i), "", bb_exit);
        auto gmem_output_buffer = new LoadInst(get_pointer_element_type(out_gep),
                                               out_gep,
                                               "gmem_output_buffer_" + std::to_string(i),
                                               false,
                                               bb_exit);
        CallInst::Create(
            reduce_smem_to_gmem_func,
            std::vector<llvm::Value*>{
                smem_output_buffer, gmem_output_buffer, ConstantInt::get(i32_type, i)},
            "",
            bb_exit);
      }
    }
  }

  ReturnInst::Create(mod->getContext(), bb_exit);

  // Resolve Forward References
  pos_inc_pre->replaceAllUsesWith(pos_inc);
  delete pos_inc_pre;

  std::error_code ec;
  raw_fd_ostream query_template_dump("query.ll", ec);
  mod->print(query_template_dump, nullptr);

  llvm::raw_os_ostream output(std::cerr);
  if (verifyFunction(*query_func_ptr, &output)) {
    LOG(FATAL) << "Generated invalid code. ";
  }

  return {query_func_ptr, row_process};
}

template <class Attributes>
std::tuple<llvm::Function*, llvm::CallInst*> query_group_by_template_impl(
    llvm::Module* mod,
    const bool hoist_literals,
    const QueryMemoryDescriptor& query_mem_desc,
    const ExecutorDeviceType device_type,
    const bool check_scan_limit,
    const GpuSharedMemoryContext& gpu_smem_context) {
  if (gpu_smem_context.isSharedMemoryUsed()) {
    CHECK(device_type == ExecutorDeviceType::GPU);
  }
  using namespace llvm;

  auto func_pos_start = pos_start<Attributes>(mod);
  CHECK(func_pos_start);
  auto func_pos_step = pos_step<Attributes>(mod);
  CHECK(func_pos_step);
  auto func_group_buff_idx = group_buff_idx<Attributes>(mod);
  CHECK(func_group_buff_idx);
  auto func_row_process = row_process<Attributes>(mod, 0, hoist_literals);
  CHECK(func_row_process);
  auto func_init_shared_mem = gpu_smem_context.isSharedMemoryUsed()
                                  ? mod->getFunction("init_shared_mem")
                                  : mod->getFunction("init_shared_mem_nop");
  CHECK(func_init_shared_mem);

  auto func_write_back = mod->getFunction("write_back_nop");
  CHECK(func_write_back);

  auto i32_type = IntegerType::get(mod->getContext(), 32);
  auto i64_type = IntegerType::get(mod->getContext(), 64);
  auto pi8_type = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);
  auto pi32_type = PointerType::get(i32_type, 0);
  auto pi64_type = PointerType::get(i64_type, 0);
  auto ppi64_type = PointerType::get(pi64_type, 0);
  auto ppi8_type = PointerType::get(pi8_type, 0);

  std::vector<Type*> query_args;
  query_args.push_back(ppi8_type);
  if (hoist_literals) {
    query_args.push_back(pi8_type);
  }
  query_args.push_back(pi64_type);
  query_args.push_back(pi64_type);
  query_args.push_back(pi32_type);
  query_args.push_back(pi64_type);

  query_args.push_back(ppi64_type);
  query_args.push_back(i32_type);
  query_args.push_back(pi64_type);
  query_args.push_back(pi32_type);
  query_args.push_back(pi32_type);

  FunctionType* query_func_type = FunctionType::get(
      /*Result=*/Type::getVoidTy(mod->getContext()),
      /*Params=*/query_args,
      /*isVarArg=*/false);

  std::string query_name{"query_group_by_template"};
  auto query_func_ptr = mod->getFunction(query_name);
  CHECK(!query_func_ptr);

  query_func_ptr = Function::Create(
      /*Type=*/query_func_type,
      /*Linkage=*/GlobalValue::ExternalLinkage,
      /*Name=*/"query_group_by_template",
      mod);

  query_func_ptr->setCallingConv(CallingConv::C);

  Attributes query_func_pal;
  {
    SmallVector<Attributes, 4> Attrs;
    Attributes PAS;
    {
      AttrBuilder B;
      B.addAttribute(Attribute::ReadNone);
      B.addAttribute(Attribute::NoCapture);
      PAS = Attributes::get(mod->getContext(), 1U, B);
    }

    Attrs.push_back(PAS);
    {
      AttrBuilder B;
      B.addAttribute(Attribute::ReadOnly);
      B.addAttribute(Attribute::NoCapture);
      PAS = Attributes::get(mod->getContext(), 2U, B);
    }

    Attrs.push_back(PAS);
    {
      AttrBuilder B;
      B.addAttribute(Attribute::ReadNone);
      B.addAttribute(Attribute::NoCapture);
      PAS = Attributes::get(mod->getContext(), 3U, B);
    }

    Attrs.push_back(PAS);
    {
      AttrBuilder B;
      B.addAttribute(Attribute::ReadOnly);
      B.addAttribute(Attribute::NoCapture);
      PAS = Attributes::get(mod->getContext(), 4U, B);
    }

    Attrs.push_back(PAS);
    {
      AttrBuilder B;
      B.addAttribute(Attribute::UWTable);
      PAS = Attributes::get(mod->getContext(), ~0U, B);
    }

    Attrs.push_back(PAS);

    query_func_pal = Attributes::get(mod->getContext(), Attrs);
  }
  query_func_ptr->setAttributes(query_func_pal);

  Function::arg_iterator query_arg_it = query_func_ptr->arg_begin();
  Value* byte_stream = &*query_arg_it;
  byte_stream->setName("byte_stream");
  Value* literals{nullptr};
  if (hoist_literals) {
    literals = &*(++query_arg_it);
    ;
    literals->setName("literals");
  }
  Value* row_count_ptr = &*(++query_arg_it);
  row_count_ptr->setName("row_count_ptr");
  Value* frag_row_off_ptr = &*(++query_arg_it);
  frag_row_off_ptr->setName("frag_row_off_ptr");
  Value* max_matched_ptr = &*(++query_arg_it);
  max_matched_ptr->setName("max_matched_ptr");
  Value* agg_init_val = &*(++query_arg_it);
  agg_init_val->setName("agg_init_val");
  Value* group_by_buffers = &*(++query_arg_it);
  group_by_buffers->setName("group_by_buffers");
  Value* frag_idx = &*(++query_arg_it);
  frag_idx->setName("frag_idx");
  Value* join_hash_tables = &*(++query_arg_it);
  join_hash_tables->setName("join_hash_tables");
  Value* total_matched = &*(++query_arg_it);
  total_matched->setName("total_matched");
  Value* error_code = &*(++query_arg_it);
  error_code->setName("error_code");

  auto bb_entry = BasicBlock::Create(mod->getContext(), ".entry", query_func_ptr, 0);
  auto bb_preheader =
      BasicBlock::Create(mod->getContext(), ".loop.preheader", query_func_ptr, 0);
  auto bb_forbody = BasicBlock::Create(mod->getContext(), ".forbody", query_func_ptr, 0);
  auto bb_crit_edge =
      BasicBlock::Create(mod->getContext(), "._crit_edge", query_func_ptr, 0);
  auto bb_exit = BasicBlock::Create(mod->getContext(), ".exit", query_func_ptr, 0);

  // Block  .entry
  LoadInst* row_count = new LoadInst(
      get_pointer_element_type(row_count_ptr), row_count_ptr, "", false, bb_entry);
  row_count->setAlignment(LLVM_ALIGN(8));
  row_count->setName("row_count");

  LoadInst* max_matched = new LoadInst(
      get_pointer_element_type(max_matched_ptr), max_matched_ptr, "", false, bb_entry);
  max_matched->setAlignment(LLVM_ALIGN(8));

  auto crt_matched_ptr = new AllocaInst(i32_type, 0, "crt_matched", bb_entry);
  auto old_total_matched_ptr = new AllocaInst(i32_type, 0, "old_total_matched", bb_entry);
  CallInst* pos_start = CallInst::Create(func_pos_start, "", bb_entry);
  pos_start->setCallingConv(CallingConv::C);
  pos_start->setTailCall(true);
  Attributes pos_start_pal;
  pos_start->setAttributes(pos_start_pal);

  CallInst* pos_step = CallInst::Create(func_pos_step, "", bb_entry);
  pos_step->setCallingConv(CallingConv::C);
  pos_step->setTailCall(true);
  Attributes pos_step_pal;
  pos_step->setAttributes(pos_step_pal);

  CallInst* group_buff_idx = CallInst::Create(func_group_buff_idx, "", bb_entry);
  group_buff_idx->setCallingConv(CallingConv::C);
  group_buff_idx->setTailCall(true);
  Attributes group_buff_idx_pal;
  group_buff_idx->setAttributes(group_buff_idx_pal);

  CastInst* pos_start_i64 = new SExtInst(pos_start, i64_type, "", bb_entry);
  const PointerType* Ty = dyn_cast<PointerType>(group_by_buffers->getType());
  CHECK(Ty);
  GetElementPtrInst* group_by_buffers_gep = GetElementPtrInst::Create(
      Ty->getElementType(), group_by_buffers, group_buff_idx, "", bb_entry);
  LoadInst* col_buffer = new LoadInst(get_pointer_element_type(group_by_buffers_gep),
                                      group_by_buffers_gep,
                                      "",
                                      false,
                                      bb_entry);
  col_buffer->setName("col_buffer");
  col_buffer->setAlignment(LLVM_ALIGN(8));

  llvm::ConstantInt* shared_mem_bytes_lv =
      ConstantInt::get(i32_type, gpu_smem_context.getSharedMemorySize());
  llvm::CallInst* result_buffer =
      CallInst::Create(func_init_shared_mem,
                       std::vector<llvm::Value*>{col_buffer, shared_mem_bytes_lv},
                       "result_buffer",
                       bb_entry);
  // TODO(Saman): change this further, normal path should not go through this

  ICmpInst* enter_or_not =
      new ICmpInst(*bb_entry, ICmpInst::ICMP_SLT, pos_start_i64, row_count, "");
  BranchInst::Create(bb_preheader, bb_exit, enter_or_not, bb_entry);

  // Block .loop.preheader
  CastInst* pos_step_i64 = new SExtInst(pos_step, i64_type, "", bb_preheader);
  BranchInst::Create(bb_forbody, bb_preheader);

  // Block .forbody
  Argument* pos_pre = new Argument(i64_type);
  PHINode* pos = PHINode::Create(i64_type, check_scan_limit ? 3 : 2, "pos", bb_forbody);

  std::vector<Value*> row_process_params;
  row_process_params.push_back(result_buffer);
  row_process_params.push_back(crt_matched_ptr);
  row_process_params.push_back(total_matched);
  row_process_params.push_back(old_total_matched_ptr);
  row_process_params.push_back(max_matched_ptr);
  row_process_params.push_back(agg_init_val);
  row_process_params.push_back(pos);
  row_process_params.push_back(frag_row_off_ptr);
  row_process_params.push_back(row_count_ptr);
  if (hoist_literals) {
    CHECK(literals);
    row_process_params.push_back(literals);
  }
  if (check_scan_limit) {
    new StoreInst(ConstantInt::get(IntegerType::get(mod->getContext(), 32), 0),
                  crt_matched_ptr,
                  bb_forbody);
  }
  CallInst* row_process =
      CallInst::Create(func_row_process, row_process_params, "", bb_forbody);
  row_process->setCallingConv(CallingConv::C);
  row_process->setTailCall(true);
  Attributes row_process_pal;
  row_process->setAttributes(row_process_pal);

  // Forcing all threads within a warp to be synchronized (Compute >= 7.x)
  if (query_mem_desc.isWarpSyncRequired(device_type)) {
    auto func_sync_warp_protected = mod->getFunction("sync_warp_protected");
    CHECK(func_sync_warp_protected);
    CallInst::Create(func_sync_warp_protected,
                     std::vector<llvm::Value*>{pos, row_count},
                     "",
                     bb_forbody);
  }

  BinaryOperator* pos_inc =
      BinaryOperator::Create(Instruction::Add, pos, pos_step_i64, "", bb_forbody);
  ICmpInst* loop_or_exit =
      new ICmpInst(*bb_forbody, ICmpInst::ICMP_SLT, pos_inc, row_count, "");
  if (check_scan_limit) {
    auto crt_matched = new LoadInst(get_pointer_element_type(crt_matched_ptr),
                                    crt_matched_ptr,
                                    "crt_matched",
                                    false,
                                    bb_forbody);
    auto filter_match = BasicBlock::Create(
        mod->getContext(), "filter_match", query_func_ptr, bb_crit_edge);
    llvm::Value* new_total_matched =
        new LoadInst(get_pointer_element_type(old_total_matched_ptr),
                     old_total_matched_ptr,
                     "",
                     false,
                     filter_match);
    new_total_matched =
        BinaryOperator::CreateAdd(new_total_matched, crt_matched, "", filter_match);
    CHECK(new_total_matched);
    ICmpInst* limit_not_reached = new ICmpInst(*filter_match,
                                               ICmpInst::ICMP_SLT,
                                               new_total_matched,
                                               max_matched,
                                               "limit_not_reached");
    BranchInst::Create(
        bb_forbody,
        bb_crit_edge,
        BinaryOperator::Create(
            BinaryOperator::And, loop_or_exit, limit_not_reached, "", filter_match),
        filter_match);
    auto filter_nomatch = BasicBlock::Create(
        mod->getContext(), "filter_nomatch", query_func_ptr, bb_crit_edge);
    BranchInst::Create(bb_forbody, bb_crit_edge, loop_or_exit, filter_nomatch);
    ICmpInst* crt_matched_nz = new ICmpInst(
        *bb_forbody, ICmpInst::ICMP_NE, crt_matched, ConstantInt::get(i32_type, 0), "");
    BranchInst::Create(filter_match, filter_nomatch, crt_matched_nz, bb_forbody);
    pos->addIncoming(pos_start_i64, bb_preheader);
    pos->addIncoming(pos_pre, filter_match);
    pos->addIncoming(pos_pre, filter_nomatch);
  } else {
    pos->addIncoming(pos_start_i64, bb_preheader);
    pos->addIncoming(pos_pre, bb_forbody);
    BranchInst::Create(bb_forbody, bb_crit_edge, loop_or_exit, bb_forbody);
  }

  // Block ._crit_edge
  BranchInst::Create(bb_exit, bb_crit_edge);

  // Block .exit
  CallInst::Create(func_write_back,
                   std::vector<Value*>{col_buffer, result_buffer, shared_mem_bytes_lv},
                   "",
                   bb_exit);

  ReturnInst::Create(mod->getContext(), bb_exit);

  // Resolve Forward References
  pos_pre->replaceAllUsesWith(pos_inc);
  delete pos_pre;

  if (verifyFunction(*query_func_ptr, &llvm::errs())) {
    LOG(FATAL) << "Generated invalid code. ";
  }

  return {query_func_ptr, row_process};
}

std::tuple<llvm::Function*, llvm::CallInst*> query_template(
    llvm::Module* module,
    const size_t aggr_col_count,
    const CompilationOptions& co,
    const bool is_estimate_query,
    const GpuSharedMemoryContext& gpu_smem_context) {
  return query_template_impl<llvm::AttributeList>(
      module, aggr_col_count, co, is_estimate_query, gpu_smem_context);
}
std::tuple<llvm::Function*, llvm::CallInst*> query_group_by_template(
    llvm::Module* module,
    const bool hoist_literals,
    const QueryMemoryDescriptor& query_mem_desc,
    const ExecutorDeviceType device_type,
    const bool check_scan_limit,
    const GpuSharedMemoryContext& gpu_smem_context) {
  return query_group_by_template_impl<llvm::AttributeList>(module,
                                                           hoist_literals,
                                                           query_mem_desc,
                                                           device_type,
                                                           check_scan_limit,
                                                           gpu_smem_context);
}
