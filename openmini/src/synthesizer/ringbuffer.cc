/// @filename ringbuffer.cc
/// @brief Implementation of a simple resizable ringbuffer
/// @author gm
/// @copyright gm 2013
///
/// This file is part of OpenMini
///
/// OpenMini is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// OpenMini is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with OpenMini.  If not, see <http://www.gnu.org/licenses/>.

// std::fill, std::copy, std::min, std::max
#include <algorithm>

#include "openmini/src/maths.h"
#include "openmini/src/synthesizer/ringbuffer.h"
#include "openmini/src/synthesizer/synthesizer_common.h"

namespace openmini {
namespace synthesizer {

RingBuffer::RingBuffer(const unsigned int capacity)
    : data_(nullptr),
      capacity_(ComputeRequiredElements(capacity)),
      size_(0),
      writing_position_(0),
      reading_position_(0) {
  OPENMINI_ASSERT(capacity > 0);
  data_ = Allocate<float>(capacity_);
  std::fill(&data_[0], &data_[capacity_], 0.0f);
}

RingBuffer::~RingBuffer() {
  Deallocate(data_);
  data_ = nullptr;
}

void RingBuffer::Pop(float* dest, const unsigned int count) {
  OPENMINI_ASSERT(IsGood());

  // Is zero padding required ?
  // Using "ints" here cause count may be < size, or the opposite
  const unsigned int zeropadding_count(static_cast<unsigned int>(
    (std::max(static_cast<int>(count - size_), 0))));
  // Actual elements count to be copied
  const unsigned int copy_count(count - zeropadding_count);
  // Length of the "right" part: from reading cursor to the buffer end
  const unsigned int right_part_size(std::min(capacity_ - reading_position_,
                                      copy_count));
  // Length of the "left" part: from the buffer beginning
  // to the last element to be copied
  const unsigned int left_part_size(copy_count - right_part_size);

  //  Copy the first part
  std::copy_n(&data_[reading_position_], right_part_size, &dest[0]);
  //  Copy the second part
  std::copy_n(&data_[0], left_part_size, &dest[right_part_size]);

  reading_position_ += copy_count;
  reading_position_ = reading_position_ % capacity_;

  size_ -= copy_count;

  // Zero-padding
  std::fill_n(&dest[copy_count],
              zeropadding_count,
              0.0f);
}

void RingBuffer::Push(SampleRead value) {
  OPENMINI_ASSERT(IsGood());

  // TODO(gm): optimize out this
  float tmp[SampleSize];
  VectorMath::Store(&tmp[0], value);
  return Push(&tmp[0], SampleSize);
}

void RingBuffer::Push(const float* const src, const unsigned int count) {
  OPENMINI_ASSERT(IsGood());
  OPENMINI_ASSERT(count <= Capacity() - Size());
  // Length of the "right" part: from writing cursor to the buffer end
  const unsigned int right_part_size(std::min(capacity_ - writing_position_,
                                              count));
  // Length of the "left" part: from the buffer beginning
  // to the last element to be pushed
  const unsigned int left_part_size(count - right_part_size);

  //  Copy the first part
  std::copy_n(&src[0], right_part_size, &data_[writing_position_]);
  //  Copy the second part
  std::copy_n(&src[right_part_size], left_part_size, &data_[0]);

  writing_position_ += count;
  writing_position_ = writing_position_ % capacity_;

  size_ += count;
}

void RingBuffer::Clear(void) {
  writing_position_ = 0;
  reading_position_ = 0;
  size_ = 0;
  if (IsGood()) {
    std::fill(&data_[0],
              &data_[Capacity()],
              0.0f);
  }
}

void RingBuffer::Reserve(const unsigned int size,
                         const unsigned int chunk_size) {
  OPENMINI_ASSERT(IsGood());

  // @todo (gm) Check that all successive size computation do not result in
  // too much data being allocated, or find a smart data size allocation scheme
  const unsigned int actual_capacity(ComputeRequiredElements(size, chunk_size));
  if (actual_capacity > Capacity()) {
    return Resize(actual_capacity);
  }
}

bool RingBuffer::IsGood(void) const {
  return data_ != nullptr;
}

unsigned int RingBuffer::Capacity(void) const {
  OPENMINI_ASSERT(IsGood());
  return capacity_;
}

unsigned int RingBuffer::Size(void) const {
  OPENMINI_ASSERT(IsGood());
  return ComputeMaxElements(size_);
}

void RingBuffer::Resize(const unsigned int size) {
  OPENMINI_ASSERT(IsGood());
  OPENMINI_ASSERT(size > 0);

  const unsigned int actual_capacity(size
  // This is the offset required in order to make future left space
  // a multiple of SampleSize.
  // E.g., IsMultiple(future capacity - currently filled data, SampleSize)
  // USELESS FOR NOW
  // + GetOffsetFromNextMultiple(size_, SampleSize)
    );
  const unsigned int max_fill_count(std::min(size_, actual_capacity));
  float* temp(Allocate<float>(actual_capacity));
  OPENMINI_ASSERT(temp != nullptr);
  std::fill(&temp[0],
            &temp[actual_capacity],
            0.0f);
  if (IsGood()) {
    // The current "interesting" part, between both position,
    // is copied to the new buffer beginning
    TransferData(&data_[reading_position_],
                 &data_[reading_position_ + max_fill_count],
                 &temp[0]);
    Deallocate(data_);
  }
  data_ = temp;
  capacity_ = actual_capacity;
  // Remember that the "interesting" part on the previous buffer
  // has been moved to the beginning of the new one !
  writing_position_ = max_fill_count;
  reading_position_ = 0;
  size_ = max_fill_count;
}

unsigned int RingBuffer::ComputeRequiredElements(
    const unsigned int size,
    const unsigned int chunk_size) const {
  // For this implementation, one internal sample = one output sample
  //
  // However one needs to take into account the chunk size,
  // which is at least the sample size
  const unsigned int actual_chunk_size(Math::Max(chunk_size, SampleSize));
  // The worst use case is that one:
  // [chunk_size - 1][...data...][chunk_size - 1]
  //                 ^ cursor1   ^ cursor2
  // hence the extra padding
  return GetNextMultiple(size + 2 * actual_chunk_size - 1, actual_chunk_size);
}

unsigned int RingBuffer::ComputeMaxElements(const unsigned int size) const {
  // For this implementation, one internal sample = one output sample
  return size;
}

void RingBuffer::TransferData(const float* const in_first,
                              const float* const in_last,
                              float* const out_first) {
  std::copy(in_first, in_last, out_first);
}

}  // namespace synthesizer
}  // namespace openmini
