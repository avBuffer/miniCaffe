#include <vector>
#include "caffe/sgd_solvers.hpp"


namespace caffe {

template <typename Dtype>
void AdaGradSolver<Dtype>::ComputeUpdateValue(int param_id, Dtype rate) {
  const vector<Blob<Dtype>*>& net_params = this->net_->learnable_params();
  const vector<float>& net_params_lr = this->net_->params_lr();
  Dtype delta = this->param_.delta();
  Dtype local_rate = rate * net_params_lr[param_id];

  switch (Caffe::mode()) {
    case Caffe::CPU: {
      // compute square of gradient in update
      caffe_powx(net_params[param_id]->count(), net_params[param_id]->cpu_diff(), Dtype(2), this->update_[param_id]->mutable_cpu_data());

      // update history
      caffe_add(net_params[param_id]->count(), this->update_[param_id]->cpu_data(), this->history_[param_id]->cpu_data(),
                this->history_[param_id]->mutable_cpu_data());

      // prepare update
      caffe_powx(net_params[param_id]->count(), this->history_[param_id]->cpu_data(), Dtype(0.5), this->update_[param_id]->mutable_cpu_data());
      caffe_add_scalar(net_params[param_id]->count(), delta, this->update_[param_id]->mutable_cpu_data());
      caffe_div(net_params[param_id]->count(), net_params[param_id]->cpu_diff(), this->update_[param_id]->cpu_data(), this->update_[param_id]->mutable_cpu_data());

      // scale and copy
      caffe_cpu_axpby(net_params[param_id]->count(), local_rate, this->update_[param_id]->cpu_data(), Dtype(0), net_params[param_id]->mutable_cpu_diff());
      break;
    }
    default:
      LOG(FATAL) << "Unknown caffe mode: " << Caffe::mode();
  }
}

INSTANTIATE_CLASS(AdaGradSolver);
REGISTER_SOLVER_CLASS(AdaGrad);

}  // namespace caffe
