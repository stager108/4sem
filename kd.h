//
// Created by Софья on 02.04.2017.
//

#ifndef RAYTRACER_KD_H
#define RAYTRACER_KD_H

#endif //RAYTRACER_KD_H

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "primitive.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <stack>
#include <cmath>
class KDTree{
private:
	int const MAX_TREE_DEPTH = 20;
	int const OBJECTS_IN_LEAF = 1;
	int const MAX_SPLITS_OF_VOXEL = 5;
	int const SPLIT_COST = 5;
	Node* root;
	Voxel bounding_box;
	struct Node{
		Node* l;
		Node* r;
		std::vector<Object> objects;

	};
	struct Voxel {
		lld x_min;
		lld y_min;
		lld z_min;
		lld x_max;
		lld y_max;
		lld z_max;
	};

public:


	Voxel	make_initial_voxel(std::vector <Object> objects,int objects_count);
	Node * rec_build( std::vector <Object> objects, int objects_count, Voxel* v, int iter);

	Node *
	make_leaf(std::vector <Object> objects,
			  int objects_count);

	void find_plane(std::vector <Object> objects, const int objects_count, const Voxel v,
			   const int tree_depth, enum Plane * const p, Point * const c);

	int	objects_in_voxel(std::vector <Object> objects, const int objects_count, const Voxel v);

	void
	split_voxel(const Voxel v, const enum Plane p, const Point c, Voxel * const vl,	Voxel * const vr);

	int	filter_overlapped_objects(std::vector <Object> objects, const int objects_count, const Voxel v);
/*
	bool
	vector_plane_intersection(const Point vector,
							  const Point vector_start,
							  const enum Plane plane,
							  const Point coord,
							  Point * const result);
*/
	bool
	voxel_intersection(const Point vector,
					   const Point vector_start,
					   const Voxel v);

	bool object_in_voxel(Object const obj, const Voxel v);

	bool point_in_voxel(const Point p, const Voxel v);

	bool find_intersection_node(Node * const node, const Voxel v, const Point vector_start, const Point vector,
						   Object const nearest_obj_ptr, Point const nearest_intersection_point_ptr,
						   lld * const nearest_intersection_point_dist_ptr);

	bool is_intersect_anything_node(Node * const node, const Voxel v, const Point vector_start, const Point vector);

	void release_kd_node(Node * node);

// Code
// --------------------------------------------------------------

/*	bool point_in_voxel(const Point p, const Voxel v) {

		return ((p.x > v.x_min) && (p.x < v.x_max) &&
				(p.y > v.y_min) && (p.y < v.y_max) &&
				(p.z > v.z_min) && (p.z < v.z_max));
	}

/*
	void
	release_kd_tree(KDTree * tree) {
		release_kd_node(tree->root);
		free(tree);
	}

	void
	release_kd_node(Node * node) {
		if(node->l)
			release_kd_node(node->l);
		if(node->r)
			release_kd_node(node->r);
		if(node->objects)
			free(node->objects);
		free(node);
	}
*/
	KDTree *
	build_kd_tree( std::vector<Object> objects,
				  int objects_count) {

		KDTree tree;
		tree.bounding_box = make_initial_voxel(objects, objects_count);
		tree.root = rec_build(objects, objects_count, tree.bounding_box, 0);
		return &tree;
	}

	Node *
	rec_build(std::vector<Point> objects,
			  int objects_count,
			  Voxel v,
			  int iter) {

		enum Plane p;
		Point c;
		find_plane(objects, objects_count, v, iter, &p, &c);

		if(p == NONE) {
			return make_leaf(objects, objects_count);
		}

		Voxel vl;
		Voxel vr;
		split_voxel(v, p, c, &vl, &vr);

		int l_objects_count = filter_overlapped_objects(objects, objects_count, vl);
		Node * l = rec_build(objects, l_objects_count, vl, iter + 1);

		int r_objects_count = filter_overlapped_objects(objects, objects_count, vr);
		Node * r = rec_build(objects, r_objects_count, vr, iter + 1);


		Node node;
		node.objects = NULL;
		node.objects_count = 0;
		node.plane = p;
		node.coord = c;
		node.l = l;
		node.r = r;

		return &node;
	}

	int filter_overlapped_objects(std::vector<Point> objects,
							  const int objects_count,
							  const Voxel v) {

		int i = 0;
		int j = objects_count - 1;

		Object * tmp;

		// Put all objects, which overlap with voxel to the left part of array
		while(i <= j) {
			while((i < j) && (object_in_voxel(objects[i], v)))
				i++;

			while((j > i) && (!object_in_voxel(objects[j], v)))
				j--;

			tmp = objects[i];
			objects[i] = objects[j];
			objects[j] = tmp;
			i++;
			j--;
		}

		// Actually, after the loop, variable 'i' is a count of overlapped objects
		return i;
	}

	void split_voxel(const Voxel v,
				const enum Plane p,
				const Point c,
				Voxel * const vl,
				Voxel * const vr) {

		*vl = v;
		*vr = v;

		switch(p) {
			case XY:
				vl->z_max = c.z;
				vr->z_min = c.z;
				break;

			case XZ:
				vl->y_max = c.y;
				vr->y_min = c.y;
				break;

			case YZ:
				vl->x_max = c.x;
				vr->x_min = c.x;
				break;
			case NONE:
				// Unreachable case
				printf("[split_voxel] Plane is NONE. Error");
				exit(1);
				break;
		}
	}

/*
 * Using Surface Area Heuristic (SAH) for finding best split pane
 *
 * SAH = 0.5 * voxel_surface_area * number_of_objects_in_voxel
 *
 * splitted_SAH = split_cost
 *                + 0.5 * left_voxel_surface_area * number_of_objects_in_left_voxel
 *                + 0.5 * right_voxel_surface_area * number_of_objects_in_right_voxel
 *
 * Finding coordinate of split plane (XY, XZ or YZ) which minimizing SAH
 *
 * If can't find optimal split plane - returns NONE
 *
 * see: http://stackoverflow.com/a/4633332/653511
 */
	void find_plane(Object ** objects,
			   const int objects_count,
			   const Voxel v,
			   const int tree_depth,
			   enum Plane * const p,
			   Point * const c) {

		if((tree_depth >= MAX_TREE_DEPTH) || (objects_count <= OBJECTS_IN_LEAF)) {
			*p = NONE;
			return;
		}

		const lld hx = v.x_max - v.x_min;
		const lld hy = v.y_max - v.y_min;
		const lld hz = v.z_max - v.z_min;

		// Calculating square of each side of initial voxel
		lld Sxy = hx * hy;
		lld Sxz = hx * hz;
		lld Syz = hy * hz;

		const lld Ssum = Sxy + Sxz + Syz;

		// Let's normalize square of each side of initial voxel
		// to satisfy the following relationship:
		// Sxy + Sxz + Syz = 1
		Sxy /= Ssum;
		Sxz /= Ssum;
		Syz /= Ssum;

		const int max_splits = MAX_SPLITS_OF_VOXEL;
		const lld split_cost = SPLIT_COST;

		// Assume that at the beginning best SAH has initial voxel
		// SAH = 0.5 * square * objects_count
		// square of initial voxel is Sxy + Sxz + Syz = 1
		lld bestSAH = objects_count;
		// initial voxel doesn't have split pane
		*p = NONE;

		lld currSAH;
		Point curr_split_coord;
		int i;
		Voxel vl;
		Voxel vr;
		lld l;
		lld r;

		lld S_split;
		lld S_non_split;

		// Let's find split surface, which have the least SAH

		// TODO: maybe do some refactoring (because of following 3 loops are very similar)

		// trying to minimize SAH by splitting across XY plane
		S_split = Sxy;
		S_non_split = Sxz + Syz;
		for(i = 1; i < max_splits; i++) {

			l = ((float) i) / max_splits;
			r = 1 - l;

			// Current coordinate of split surface
			curr_split_coord.z = v.z_min + l * hz;

			split_voxel(v, XY, curr_split_coord, &vl, &vr);

			currSAH = (S_split + l * S_non_split) * objects_in_voxel(objects, objects_count, vl)
					  + (S_split + r * S_non_split) * objects_in_voxel(objects, objects_count, vr)
					  + split_cost;

			if(currSAH < bestSAH) {
				bestSAH = currSAH;
				*p = XY;
				*c = curr_split_coord;
			}
		}

		// trying to minimize SAH by splitting across XZ plane
		S_split = Sxz;
		S_non_split = Sxy + Syz;
		for(i = 1; i < max_splits; i++) {

			l = ((float) i) / max_splits;
			r = 1 - l;

			// Current coordinate of split surface
			curr_split_coord.y = v.y_min + l * hy;

			split_voxel(v, XZ, curr_split_coord, &vl, &vr);

			currSAH = (S_split + l * S_non_split) * objects_in_voxel(objects, objects_count, vl)
					  + (S_split + r * S_non_split) * objects_in_voxel(objects, objects_count, vr)
					  + split_cost;

			if(currSAH < bestSAH) {
				bestSAH = currSAH;
				*p = XZ;
				*c = curr_split_coord;
			}
		}

		// trying to minimize SAH by splitting across YZ plane
		S_split = Syz;
		S_non_split = Sxy + Sxz;
		for(i = 1; i < max_splits; i++) {

			l = ((float) i) / max_splits;
			r = 1 - l;

			// Current coordinate of split surface
			curr_split_coord.x = v.x_min + l * hx;

			split_voxel(v, YZ, curr_split_coord, &vl, &vr);

			currSAH = (S_split + l * S_non_split) * objects_in_voxel(objects, objects_count, vl)
					  + (S_split + r * S_non_split) * objects_in_voxel(objects, objects_count, vr)
					  + split_cost;

			if(currSAH < bestSAH) {
				bestSAH = currSAH;
				*p = YZ;
				*c = curr_split_coord;
			}
		}
	}

	int	objects_in_voxel(std::vector <Object> objects,
					 const int objects_count,
					 const Voxel v) {

		int i;
		int count = 0;
		for(i = 0; i < objects_count; i++)
			if(object_in_voxel(objects[i], v))
				++count;

		return count;
	}

	Voxel
	make_initial_voxel(std::vector <Object> objects,
					   int objects_count) {

		if(!objects_count) {
			Voxel v = {-1, -1, -1, 1, 1, 1};
			return v;
		}

		Point min_p = objects[0]->get_min_boundary_point(objects[0]->data);
		Point max_p = objects[0]->get_max_boundary_point(objects[0]->data);

		lld x_min = min_p.x;
		lld y_min = min_p.y;
		lld z_min = min_p.z;

		lld x_max = max_p.x;
		lld y_max = max_p.y;
		lld z_max = max_p.z;

		int i;
		for(i = 0; i < objects_count; i++) {
			min_p = objects[i]->get_min_boundary_point(objects[i]->data);
			max_p = objects[i]->get_max_boundary_point(objects[i]->data);

			x_min = (x_min < min_p.x) ? x_min : min_p.x;
			y_min = (y_min < min_p.y) ? y_min : min_p.y;
			z_min = (z_min < min_p.z) ? z_min : min_p.z;

			x_max = (x_max > max_p.x) ? x_max : max_p.x;
			y_max = (y_max > max_p.y) ? y_max : max_p.y;
			z_max = (z_max > max_p.z) ? z_max : max_p.z;
		}

		Voxel v = {x_min - 1, y_min - 1, z_min - 1, x_max + 1, y_max + 1, z_max + 1};
		return v;
	}


	inline __hot Boolean
	object_in_voxel(Object * const obj,
	const Voxel v) {

		Point min_p = obj->get_min_boundary_point(obj->data);
		Point max_p = obj->get_max_boundary_point(obj->data);

		return
				!((max_p.x < v.x_min)
				  || (max_p.y < v.y_min)
				  || (max_p.z < v.z_min)
				  || (min_p.x > v.x_max)
				  || (min_p.y > v.y_max)
				  || (min_p.z > v.z_max));
	}

	inline Node *
	make_leaf(std::vector<Object> objects,
			  int objects_count) {

		Node * leaf = malloc(sizeof(Node));
		leaf->plane = NONE;
		leaf->objects_count = objects_count;
		leaf->l = NULL;
		leaf->r = NULL;
		if(objects_count) {
			leaf->objects = (Object3d **) malloc(objects_count * sizeof(Object3d *));
		} else {
			leaf->objects = NULL;
		}

		memcpy(leaf->objects, objects, objects_count * sizeof(Object3d *));

		return leaf;
	}

	inline __hot Boolean
	vector_plane_intersection(const Point vector,
							  const Point vector_start,
							  const enum Plane plane,
							  const Point coord,
							  Point * const result) {

		lld k;
		switch(plane) {
			case XY:
				if(((coord.z < vector_start.z) && (vector.z > 0))
				   || ((coord.z > vector_start.z) && (vector.z < 0)))
					return False;

				k = (coord.z - vector_start.z) / vector.z;
				*result = Point(vector_start.x + vector.x * k,
								  vector_start.y + vector.y * k,
								  coord.z);
				break;

			case XZ:
				if(((coord.y < vector_start.y) && (vector.y > 0))
				   || ((coord.y > vector_start.y) && (vector.y < 0)))
					return False;

				k = (coord.y - vector_start.y) / vector.y;
				*result = Point(vector_start.x + vector.x * k,
								  coord.y,
								  vector_start.z + vector.z * k);
				break;

			case YZ:
				if(((coord.x < vector_start.x) && (vector.x > 0))
				   || ((coord.x > vector_start.x) && (vector.x < 0)))
					return False;

				k = (coord.x - vector_start.x) / vector.x;
				*result = Point(coord.x,
								  vector_start.y + vector.y * k,
								  vector_start.z + vector.z * k);
				break;

			case NONE:
				// Unreachable case
				printf("[vector_plane_intersection] Plane is NONE. Error");
				exit(1);
				break;
		}

		return True;
	}

	bool
	voxel_intersection(const Point vector, const Point vector_start, const Voxel v) {

		if(point_in_voxel(vector_start, v))
			return True;

		Point p;
		Point c;

		c.z = v.z_min;
		if(vector_plane_intersection(vector, vector_start, XY, c, &p)
		   && (p.x > v.x_min) && (p.x < v.x_max)&& (p.y > v.y_min) && (p.y < v.y_max)) {
			return True;
		}

		c.z = v.z_max;
		if(vector_plane_intersection(vector, vector_start, XY, c, &p)
		   && (p.x > v.x_min) && (p.x < v.x_max)&& (p.y > v.y_min) && (p.y < v.y_max)) {
			return True;
		}

		c.y = v.y_min;
		if(vector_plane_intersection(vector, vector_start, XZ, c, &p)
		   && (p.x > v.x_min) && (p.x < v.x_max) && (p.z > v.z_min) && (p.z < v.z_max)) {
			return True;
		}

		c.y = v.y_max;
		if(vector_plane_intersection(vector, vector_start, XZ, c, &p)
		   && (p.x > v.x_min) && (p.x < v.x_max) && (p.z > v.z_min) && (p.z < v.z_max)) {
			return True;
		}

		c.x = v.x_min;
		if(vector_plane_intersection(vector, vector_start, YZ, c, &p)
		   && (p.y > v.y_min) && (p.y < v.y_max) && (p.z > v.z_min) && (p.z < v.z_max)) {
			return True;
		}

		c.x = v.x_max;
		if(vector_plane_intersection(vector, vector_start, YZ, c, &p)
		   && (p.y > v.y_min) && (p.y < v.y_max) && (p.z > v.z_min) && (p.z < v.z_max)) {
			return True;
		}
		return False;
	}

	bool
	find_intersection_tree(KDTree * const tree, const Point vector_start, const Point vector,
						   std::vector <Object> objects const nearest_obj_ptr,
						   Point * const nearest_intersection_point_ptr,
						   lld * const nearest_intersection_point_dist_ptr) {

#ifndef NO_BOUNDING_BOX
		return (voxel_intersection(vector, vector_start, tree->bounding_box)
				&& find_intersection_node(tree->root, tree->bounding_box, vector_start, vector, nearest_obj_ptr,
										  nearest_intersection_point_ptr, nearest_intersection_point_dist_ptr));
#else
		// Do not take into account scene bounds
    return find_intersection_node(tree->root,
                                      tree->bounding_box,
                                      vector_start,
                                      vector,
                                      nearest_obj_ptr,
                                      nearest_intersection_point_ptr,
                                      nearest_intersection_point_dist_ptr);
#endif // NO_BOUNDING_BOX
	}

	bool
	find_intersection_node(Node * const node,  const Voxel v, const Point vector_start, const Point vector,
						   std::vector <Object> objects const nearest_obj_ptr,
						   Point * const nearest_intersection_point_ptr,
						   lld * const nearest_intersection_point_dist_ptr) {

		// Is leaf
		if(node->plane == NONE) {
			if((node->objects_count) && (node->objects)) {
				int i;

				Object3d * obj = NULL;
				Point intersection_point;
				lld sqr_curr_dist;

				Object3d * nearest_obj = NULL;
				Point nearest_intersection_point;
				lld sqr_nearest_dist;

				int intersected = False;

				// Finding nearest object
				// and intersection point
				for(i = 0; i < node->objects_count; i++) {
					if(node->objects[i]) {
						obj = node->objects[i];

#ifdef RAY_INTERSECTIONS_STAT
						++intersections_per_ray;
#endif // RAY_INTERSECTIONS_STAT

						if((obj->intersect(obj->data, vector_start, vector, &intersection_point))
						   && (point_in_voxel(intersection_point, v))) {

							sqr_curr_dist = sqr_module_vector(vector3dp(vector_start, intersection_point));

							if((sqr_curr_dist < sqr_nearest_dist) || (!intersected)) {

								nearest_obj = obj;
								nearest_intersection_point = intersection_point;
								sqr_nearest_dist = sqr_curr_dist;
								intersected = True;
							}
						}
					}
				}

				if(intersected) {
					lld nearest_dist = sqrt(sqr_nearest_dist);

					if(nearest_dist < *nearest_intersection_point_dist_ptr) {
						*nearest_intersection_point_dist_ptr = nearest_dist;
						*nearest_obj_ptr = nearest_obj;
						*nearest_intersection_point_ptr = nearest_intersection_point;
					}
				}

				return intersected;
			}
			return False;
		}

		// Otherwise

		Voxel front_voxel;
		Voxel back_voxel;

		Node * front_node;
		Node * back_node;

		switch(node->plane) {
			case XY:
				if(((node->coord.z > v.z_min) && (node->coord.z > vector_start.z))
				   || ((node->coord.z < v.z_min) && (node->coord.z < vector_start.z))) {

					front_node = node->l;
					back_node = node->r;
					split_voxel(v, node->plane, node->coord, &front_voxel, &back_voxel);
				} else {
					front_node = node->r;
					back_node = node->l;
					split_voxel(v, node->plane, node->coord, &back_voxel, &front_voxel);
				}
				break;

			case XZ:
				if(((node->coord.y > v.y_min) && (node->coord.y > vector_start.y))
				   || ((node->coord.y < v.y_min) && (node->coord.y < vector_start.y))) {

					front_node = node->l;
					back_node = node->r;
					split_voxel(v, node->plane, node->coord, &front_voxel, &back_voxel);
				} else {
					front_node = node->r;
					back_node = node->l;
					split_voxel(v, node->plane, node->coord, &back_voxel, &front_voxel);
				}
				break;

			case YZ:
				if(((node->coord.x > v.x_min) && (node->coord.x > vector_start.x))
				   || ((node->coord.x < v.x_min) && (node->coord.x < vector_start.x))) {

					front_node = node->l;
					back_node = node->r;
					split_voxel(v, node->plane, node->coord, &front_voxel, &back_voxel);
				} else {
					front_node = node->r;
					back_node = node->l;
					split_voxel(v, node->plane, node->coord, &back_voxel, &front_voxel);
				}
				break;

			case NONE:
				// Unreachable case
				printf("[find_intersection_node] Plane is NONE. Error");
				exit(1);
				break;
		}

		if(voxel_intersection(vector, vector_start, front_voxel)
		   && find_intersection_node(front_node, front_voxel, vector_start, vector, nearest_obj_ptr,
									 nearest_intersection_point_ptr, nearest_intersection_point_dist_ptr))
			return True;

		return (voxel_intersection(vector, vector_start, back_voxel)
				&& find_intersection_node(back_node, back_voxel, vector_start, vector, nearest_obj_ptr,
										  nearest_intersection_point_ptr, nearest_intersection_point_dist_ptr));





};