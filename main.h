// Requires mri.h, model.h

class MainWindow : public Fl_Window {
 public:
  explicit MainWindow(const Mri &mri, Model &model);
  void redrawMri();

 private:
  Model &model_;
  Fl_Group *const coronalGroup_;
  Fl_Group *const sagittalGroup_;
  Fl_Group *const axialGroup_;
  CoronalWindow *const coronal_;
  SagittalWindow *const sagittal_;
  AxialWindow *const axial_;

  int handle(int event);
};